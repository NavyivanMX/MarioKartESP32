/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : StatusLightController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del controlador de la luz de estado del transmisor.
 ******************************************************************************/

#include "StatusLightController.h"

namespace MK
{

//=============================================================================
// Constantes locales
//=============================================================================

namespace
{

constexpr std::uint32_t StartupBlinkIntervalMs = 150;

constexpr std::uint32_t ProfileBlinkIntervalMs = 150;

constexpr std::uint8_t ProfileBlinkCount = 3;

// Turbo normal.
// El efecto avanza rápidamente por fase.
constexpr std::uint32_t TurboUpdateIntervalMs = 20;

// Police:
// alternancia azul / rojo.
constexpr std::uint32_t PoliceTurboIntervalMs = 80;

}

//=============================================================================
// Ciclo de vida
//=============================================================================

bool StatusLightController::Begin() noexcept
{
    if (m_started)
    {
        return true;
    }

    //-------------------------------------------------------------------------
    // NeoPixel
    //-------------------------------------------------------------------------

    m_strip.begin();

    m_strip.setBrightness(255);

    Clear();

    //-------------------------------------------------------------------------
    // Estado general
    //-------------------------------------------------------------------------

    m_started = true;

    //-------------------------------------------------------------------------
    // Startup
    //-------------------------------------------------------------------------

    m_startupActive = true;
    m_startupBlinkOn = false;
    m_startupBlinkCount = 0;
    m_startupLastUpdate = millis();

    //-------------------------------------------------------------------------
    // Perfil
    //-------------------------------------------------------------------------

    m_hasProfile = false;

    m_profileEffectActive = false;
    m_profileBlinkOn = false;
    m_profileBlinkCount = 0;
    m_profileLastUpdate = 0;

    //-------------------------------------------------------------------------
    // Turbo
    //-------------------------------------------------------------------------

    m_turboActive = false;
    m_turboPhase = 0;
    m_turboLastUpdate = 0;

    //-------------------------------------------------------------------------
    // Police Turbo
    //-------------------------------------------------------------------------

    m_policeTurboState = false;
    m_policeTurboLastUpdate = 0;

    return true;
}

//=============================================================================
// Update
//=============================================================================

void StatusLightController::Update(
    const Protocol::DriverCommand& command,
    VehicleProfiles::DrivingProfileId profile) noexcept
{
    if (!m_started)
    {
        return;
    }

    //-------------------------------------------------------------------------
    // Detectar primer perfil recibido
    //-------------------------------------------------------------------------

    if (!m_hasProfile)
    {
        m_currentProfile = profile;
        m_lastProfile = profile;

        m_hasProfile = true;
    }

    //-------------------------------------------------------------------------
    // Detectar cambio de perfil
    //-------------------------------------------------------------------------

    else if (profile != m_lastProfile)
    {
        m_lastProfile = profile;
        m_currentProfile = profile;

        m_profileEffectActive = true;
        m_profileBlinkOn = false;
        m_profileBlinkCount = 0;
        m_profileLastUpdate = millis();

        // Reiniciar turbo.
        m_turboActive = false;
        m_turboPhase = 0;

        m_policeTurboState = false;
        m_policeTurboLastUpdate = 0;

        Clear();
    }

    //-------------------------------------------------------------------------
    // Startup
    //-------------------------------------------------------------------------

    if (m_startupActive)
    {
        UpdateStartupEffect();
        return;
    }

    //-------------------------------------------------------------------------
    // Cambio de perfil
    //-------------------------------------------------------------------------

    if (m_profileEffectActive)
    {
        UpdateProfileEffect();
        return;
    }

    //-------------------------------------------------------------------------
    // Turbo
    //-------------------------------------------------------------------------

    using Types::Vehicle::Turbo;

    const bool turbo =
        command.turbo == Turbo::Enabled;

    //-------------------------------------------------------------------------
    // Turbo OFF
    //-------------------------------------------------------------------------

    if (!turbo)
    {
        if (m_turboActive)
        {
            m_turboActive = false;

            m_turboPhase = 0;

            m_policeTurboState = false;

            Clear();
        }

        return;
    }

    //-------------------------------------------------------------------------
    // Turbo ON
    //-------------------------------------------------------------------------

    if (IsPoliceProfile(m_currentProfile))
    {
        UpdatePoliceTurboEffect();
    }
    else
    {
        UpdateTurboEffect();
    }
}

//=============================================================================
// Startup
//=============================================================================

void StatusLightController::UpdateStartupEffect() noexcept
{
    if (!m_hasProfile)
    {
        return;
    }

    const std::uint32_t now =
        millis();

    if ((now - m_startupLastUpdate) <
        StartupBlinkIntervalMs)
    {
        return;
    }

    m_startupLastUpdate = now;

    //-------------------------------------------------------------------------
    // Cambiar estado
    //-------------------------------------------------------------------------

    m_startupBlinkOn =
        !m_startupBlinkOn;

    if (m_startupBlinkOn)
    {
        std::uint8_t red;
        std::uint8_t green;
        std::uint8_t blue;

        GetProfileColor(
            m_currentProfile,
            red,
            green,
            blue);

        SetColor(
            red,
            green,
            blue);

        ++m_startupBlinkCount;
    }
    else
    {
        Clear();
    }

    //-------------------------------------------------------------------------
    // Dos blinks
    //-------------------------------------------------------------------------

    if (m_startupBlinkCount >= 2 &&
        !m_startupBlinkOn)
    {
        m_startupActive = false;

        Clear();
    }
}

//=============================================================================
// Cambio de perfil
//=============================================================================

void StatusLightController::UpdateProfileEffect() noexcept
{
    const std::uint32_t now =
        millis();

    if ((now - m_profileLastUpdate) <
        ProfileBlinkIntervalMs)
    {
        return;
    }

    m_profileLastUpdate = now;

    //-------------------------------------------------------------------------
    // Cambiar estado
    //-------------------------------------------------------------------------

    m_profileBlinkOn =
        !m_profileBlinkOn;

    if (m_profileBlinkOn)
    {
        std::uint8_t red;
        std::uint8_t green;
        std::uint8_t blue;

        GetProfileColor(
            m_currentProfile,
            red,
            green,
            blue);

        SetColor(
            red,
            green,
            blue);

        ++m_profileBlinkCount;
    }
    else
    {
        Clear();
    }

    //-------------------------------------------------------------------------
    // Tres blinks
    //-------------------------------------------------------------------------

    if (m_profileBlinkCount >= ProfileBlinkCount &&
        !m_profileBlinkOn)
    {
        m_profileEffectActive = false;

        Clear();
    }
}

//=============================================================================
// Turbo normal
//=============================================================================

void StatusLightController::UpdateTurboEffect() noexcept
{
    const std::uint32_t now =
        millis();

    if ((now - m_turboLastUpdate) <
        TurboUpdateIntervalMs)
    {
        return;
    }

    m_turboLastUpdate = now;

    m_turboActive = true;

    //---------------------------------------------------------------------
    // Misma idea de animación turbo del RearLightController.
    //
    // Usamos el perfil actual como color base.
    //---------------------------------------------------------------------

    std::uint8_t red;
    std::uint8_t green;
    std::uint8_t blue;

    GetProfileColor(
        m_currentProfile,
        red,
        green,
        blue);

    //---------------------------------------------------------------------
    // Variación de brillo.
    //---------------------------------------------------------------------

    const std::uint8_t level =
        static_cast<std::uint8_t>(
            80 +
            ((m_turboPhase % 6) * 35));

    const std::uint16_t scaledRed =
        (static_cast<std::uint16_t>(red) * level) / 255;

    const std::uint16_t scaledGreen =
        (static_cast<std::uint16_t>(green) * level) / 255;

    const std::uint16_t scaledBlue =
        (static_cast<std::uint16_t>(blue) * level) / 255;

    SetColor(
        static_cast<std::uint8_t>(scaledRed),
        static_cast<std::uint8_t>(scaledGreen),
        static_cast<std::uint8_t>(scaledBlue));

    //---------------------------------------------------------------------
    // Avanzar animación.
    //---------------------------------------------------------------------

    m_turboPhase =
        m_turboPhase + 5;
}

//=============================================================================
// Turbo Police
//=============================================================================

void StatusLightController::UpdatePoliceTurboEffect() noexcept
{
    const std::uint32_t now =
        millis();

    if ((now - m_policeTurboLastUpdate) <
        PoliceTurboIntervalMs)
    {
        return;
    }

    m_policeTurboLastUpdate = now;

    m_turboActive = true;

    //---------------------------------------------------------------------
    // Alternar azul / rojo.
    //---------------------------------------------------------------------

    m_policeTurboState =
        !m_policeTurboState;

    if (m_policeTurboState)
    {
        // Azul
        SetColor(
            0,
            0,
            255);
    }
    else
    {
        // Rojo
        SetColor(
            255,
            0,
            0);
    }
}

//=============================================================================
// Perfil
//=============================================================================

bool StatusLightController::IsPoliceProfile(
    VehicleProfiles::DrivingProfileId profile) const noexcept
{
    return profile ==
        VehicleProfiles::DrivingProfileId::Police;
}

//-----------------------------------------------------------------------------

void StatusLightController::GetProfileColor(
    VehicleProfiles::DrivingProfileId profile,
    std::uint8_t& red,
    std::uint8_t& green,
    std::uint8_t& blue) const noexcept
{
    red = 0;
    green = 0;
    blue = 0;

    using VehicleProfiles::DrivingProfileId;

    switch (profile)
    {
        case DrivingProfileId::Rookie:
            green = 255;
            break;

        case DrivingProfileId::Normal:
            blue = 255;
            break;

        case DrivingProfileId::Advanced:
            red = 255;
            green = 80;
            break;

        case DrivingProfileId::Drift:
            red = 180;
            blue = 255;
            break;

        case DrivingProfileId::Police:
            blue = 255;
            break;

        default:
            break;
    }
}

//=============================================================================
// LED
//=============================================================================

void StatusLightController::SetColor(
    std::uint8_t red,
    std::uint8_t green,
    std::uint8_t blue) noexcept
{
    m_strip.setPixelColor(
        0,
        m_strip.Color(
            red,
            green,
            blue));

    m_strip.show();
}

//-----------------------------------------------------------------------------

void StatusLightController::SetWhite() noexcept
{
    SetColor(
        255,
        255,
        255);
}

//-----------------------------------------------------------------------------

void StatusLightController::Clear() noexcept
{
    m_strip.clear();
    m_strip.show();
}

//=============================================================================
// Stop
//=============================================================================

void StatusLightController::Stop() noexcept
{
    if (!m_started)
    {
        return;
    }

    m_startupActive = false;
    m_profileEffectActive = false;
    m_turboActive = false;

    m_startupBlinkOn = false;
    m_profileBlinkOn = false;

    m_turboPhase = 0;
    m_policeTurboState = false;

    Clear();
}

} // namespace MK