/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : RearLightController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del controlador de las luces traseras.
 ******************************************************************************/

#include "RearLightController.h"

#include <cstring>

namespace MK
{

//=============================================================================
// Begin
//=============================================================================

bool RearLightController::Begin() noexcept
{
    m_strip.begin();

    m_strip.setBrightness(100);

    Clear();

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

    m_profileEffectActive = false;
    m_profileBlinkOn = false;
    m_profileBlinkCount = 0;
    m_profileLastUpdate = millis();

    //-------------------------------------------------------------------------
    // Turbo
    //-------------------------------------------------------------------------

    m_turboPhase = 0;
    m_turboLastUpdate = millis();

    //-------------------------------------------------------------------------
    // Police Turbo
    //-------------------------------------------------------------------------

    m_policeTurboState = false;
    m_policeTurboLastUpdate = millis();
 
    return true;
}

//=============================================================================
// Stop
//=============================================================================

void RearLightController::Stop() noexcept
{
    //-------------------------------------------------------------------------
    // Turbo
    //-------------------------------------------------------------------------

    m_turboPhase = 0;
    m_turboLastUpdate = millis();

    //-------------------------------------------------------------------------
    // Profile effect
    //-------------------------------------------------------------------------

    m_profileEffectActive = false;
    m_profileBlinkOn = false;
    m_profileBlinkCount = 0;
    m_profileLastUpdate = millis();

    //-------------------------------------------------------------------------
    // Police turbo
    //-------------------------------------------------------------------------

    m_policeTurboState = false;
    m_policeTurboLastUpdate = millis();

    //-------------------------------------------------------------------------
    // LEDs OFF
    //-------------------------------------------------------------------------

    Clear();
}

//=============================================================================
// Update
//=============================================================================

void RearLightController::Update(
    const Protocol::DriverCommand& command,
    const VehicleProfiles::DrivingProfile& profile) noexcept
{

    if (!m_started)
    {
        return;
    }
    //=========================================================================
    // Detectar cambio de perfil
    //=========================================================================

    const bool profileChanged =
        !m_hasLastProfile ||
        std::strcmp(
            profile.name,
            m_lastProfileName) != 0;

    if (profileChanged)
    {
        m_lastProfileName = profile.name;
        m_hasLastProfile = true;

        //=====================================================================
        // El startup no genera efecto de cambio
        //=====================================================================

        if (!m_startupActive)
        {
            m_profileEffectActive = true;

            m_profileBlinkOn = false;
            m_profileBlinkCount = 0;
            m_profileLastUpdate = millis();

            m_policeTurboState = false;
            m_policeTurboLastUpdate = millis();
        }
    }

    //=========================================================================
    // PRIORIDAD 1
    //
    // REVERSA = BLANCO
    //=========================================================================

    if (command.direction ==
        Types::Vehicle::Direction::Reverse)
    {
        m_startupActive = false;
        m_profileEffectActive = false;

        SetWhite();

        return;
    }

    //=========================================================================
    // PRIORIDAD 2
    //
    // STARTUP
    //=========================================================================

    if (m_startupActive)
    {
        UpdateStartupEffect();

        return;
    }

    //=========================================================================
    // PRIORIDAD 3
    //
    // CAMBIO DE PERFIL
    //=========================================================================

    if (m_profileEffectActive)
    {
        UpdateProfileEffect(profile);

        return;
    }

    //=========================================================================
    // PRIORIDAD 4
    //
    // TURBO
    //=========================================================================

    if (command.turbo ==
        Types::Vehicle::Turbo::Enabled)
    {
        UpdateTurboEffect(profile);

        return;
    }

    //=========================================================================
    // ESTADO NORMAL
    //=========================================================================

    Clear();
}

//=============================================================================
// Startup Effect
//=============================================================================

void RearLightController::UpdateStartupEffect() noexcept
{
    constexpr std::uint32_t BlinkIntervalMs = 180;

    const std::uint32_t now = millis();

    if ((now - m_startupLastUpdate) <
        BlinkIntervalMs)
    {
        return;
    }

    m_startupLastUpdate = now;

    //=========================================================================
    // Encender
    //=========================================================================

    if (!m_startupBlinkOn)
    {
        std::uint8_t red = 0;
        std::uint8_t green = 0;
        std::uint8_t blue = 0;

        //=====================================================================
        // Color del perfil
        //=====================================================================

        if (m_hasLastProfile)
        {
            if (std::strcmp(m_lastProfileName,"Rookie") == 0)
            {
                red = 0;
                green = 255;
                blue = 0;
            }
            else if (std::strcmp(m_lastProfileName,"Normal") == 0)
            {
                red = 0;
                green = 0;
                blue = 255;
            }
            else if (std::strcmp(m_lastProfileName,"Advanced") == 0)
            {
                red = 255;
                green = 80;
                blue = 0;
            }
            else if (std::strcmp(m_lastProfileName,"Drift") == 0)
            {
                red = 180;
                green = 0;
                blue = 255;
            }
            else if (std::strcmp(m_lastProfileName,"Police") == 0)
            {
                red = 0;
                green = 0;
                blue = 255;
            }
        }

        SetColor(
            red,
            green,
            blue);

        m_startupBlinkOn = true;

        return;
    }

    //=========================================================================
    // Apagar
    //=========================================================================

    Clear();

    m_startupBlinkOn = false;

    ++m_startupBlinkCount;

    //=========================================================================
    // Terminar después de 2 blink
    //=========================================================================

    if (m_startupBlinkCount >= 2)
    {
        m_startupActive = false;

        m_startupBlinkCount = 0;
        m_startupBlinkOn = false;

        Clear();
    }
}

//=============================================================================
// Profile Effect
//=============================================================================

void RearLightController::UpdateProfileEffect(
    const VehicleProfiles::DrivingProfile& profile) noexcept
{
    //=========================================================================
    // Police
    //=========================================================================

    if (IsPoliceProfile(profile))
    {
        UpdatePoliceProfileEffect();

        return;
    }

    //=========================================================================
    // Perfiles normales
    //=========================================================================

    constexpr std::uint32_t BlinkIntervalMs = 180;

    const std::uint32_t now = millis();

    if ((now - m_profileLastUpdate) <
        BlinkIntervalMs)
    {
        return;
    }

    m_profileLastUpdate = now;

    //=========================================================================
    // Encender
    //=========================================================================

    if (!m_profileBlinkOn)
    {
        std::uint8_t red = 0;
        std::uint8_t green = 0;
        std::uint8_t blue = 0;

        GetProfileColor(
            profile,
            red,
            green,
            blue);

        SetColor(
            red,
            green,
            blue);

        m_profileBlinkOn = true;

        return;
    }

    //=========================================================================
    // Apagar
    //=========================================================================

    Clear();

    m_profileBlinkOn = false;

    ++m_profileBlinkCount;

    //=========================================================================
    // Tres blink
    //=========================================================================

    if (m_profileBlinkCount >= 3)
    {
        m_profileEffectActive = false;

        m_profileBlinkCount = 0;
        m_profileBlinkOn = false;

        Clear();
    }
}

//=============================================================================
// Police Profile Effect
//=============================================================================

void RearLightController::UpdatePoliceProfileEffect() noexcept
{

    constexpr std::uint32_t BlinkIntervalMs = 180;

    const std::uint32_t now = millis();

    if ((now - m_profileLastUpdate) <
        BlinkIntervalMs)
    {
        return;
    }

    m_profileLastUpdate = now;

    //=========================================================================
    // Apagar después de cada blink
    //=========================================================================

    if (m_profileBlinkOn)
    {
        Clear();

        m_profileBlinkOn = false;

        ++m_profileBlinkCount;

        if (m_profileBlinkCount >= 3)
        {
            m_profileEffectActive = false;

            m_profileBlinkCount = 0;
            m_profileBlinkOn = false;

            Clear();
        }

        return;
    }

    //=========================================================================
    // Azul
    //=========================================================================

    if ((m_profileBlinkCount % 2) == 0)
    {
        SetLedColor(
            0,
            0,
            0,
            255);

        SetLedColor(
            1,
            255,
            0,
            0);
    }

    //=========================================================================
    // Rojo
    //=========================================================================

    else
    {
        SetLedColor(
            0,
            0,
            0,
            255);

        SetLedColor(
            1,
            255,
            0,
            0);
    }

    m_profileBlinkOn = true;
}

//=============================================================================
// Turbo Effect
//=============================================================================

void RearLightController::UpdateTurboEffect(
    const VehicleProfiles::DrivingProfile& profile) noexcept
{
    //=========================================================================
    // Police
    //=========================================================================

    if (IsPoliceProfile(profile))
    {

        UpdatePoliceTurboEffect();

        return;
    }

    //=========================================================================
    // Turbo normal
    //=========================================================================

    constexpr std::uint32_t FrameIntervalMs = 3;

    const std::uint32_t now = millis();

    if ((now - m_turboLastUpdate) <
        FrameIntervalMs)
    {
        return;
    }

    m_turboLastUpdate = now;

    //=========================================================================
    // Avance de fase
    //
    // Ajuste realizado:
    //
    // m_turboPhase = m_turboPhase + 5;
    //=========================================================================

    m_turboPhase =
        m_turboPhase + 5;

    if (m_turboPhase >= 510)
    {
        m_turboPhase = 0;
    }

    //=========================================================================
    // Variables RGB
    //=========================================================================

    std::uint8_t red = 0;
    std::uint8_t green = 0;
    std::uint8_t blue = 0;

    //=========================================================================
    // Naranja -> Rojo
    //=========================================================================

    if (m_turboPhase < 128)
    {
        const std::uint8_t greenValue =
            static_cast<std::uint8_t>(
                80 -
                (
                    static_cast<std::uint16_t>(80) *
                    m_turboPhase
                ) /
                127);

        red = 255;
        green = greenValue;
        blue = 0;
    }

    //=========================================================================
    // Rojo -> Morado
    //=========================================================================

    else if (m_turboPhase < 256)
    {
        const std::uint16_t phase =
            m_turboPhase - 128;

        red = 255;
        green = 0;

        blue =
            static_cast<std::uint8_t>(
                (
                    static_cast<std::uint16_t>(255) *
                    phase
                ) /
                127);
    }

    //=========================================================================
    // Morado -> Azul
    //=========================================================================

    else if (m_turboPhase < 384)
    {
        const std::uint16_t phase =
            m_turboPhase - 256;

        red =
            static_cast<std::uint8_t>(
                255 -
                (
                    static_cast<std::uint16_t>(255) *
                    phase
                ) /
                127);

        green = 0;
        blue = 255;
    }

    //=========================================================================
    // Azul -> Morado -> Rojo
    //=========================================================================

    else
    {
        const std::uint16_t phase =
            m_turboPhase - 384;

        red =
            static_cast<std::uint8_t>(
                (
                    static_cast<std::uint16_t>(255) *
                    phase
                ) /
                125);

        green = 0;

        blue =
            static_cast<std::uint8_t>(
                255 -
                (
                    static_cast<std::uint16_t>(255) *
                    phase
                ) /
                125);
    }

    //=========================================================================
    // Brillo dinámico
    //=========================================================================

    std::uint16_t brightness = 0;

    if (m_turboPhase < 255)
    {
        brightness =
            80 +
            (
                static_cast<std::uint16_t>(175) *
                m_turboPhase
            ) /
            254;
    }
    else
    {
        brightness =
            255 -
            (
                static_cast<std::uint16_t>(175) *
                (m_turboPhase - 255)
            ) /
            254;
    }

    //=========================================================================
    // Aplicar brillo
    //=========================================================================

    red =
        static_cast<std::uint8_t>(
            (
                static_cast<std::uint16_t>(red) *
                brightness
            ) /
            255);

    green =
        static_cast<std::uint8_t>(
            (
                static_cast<std::uint16_t>(green) *
                brightness
            ) /
            255);

    blue =
        static_cast<std::uint8_t>(
            (
                static_cast<std::uint16_t>(blue) *
                brightness
            ) /
            255);

    //=========================================================================
    // Ambos LEDs
    //=========================================================================

    SetColor(
        red,
        green,
        blue);
}

//=============================================================================
// Police Turbo Effect
//=============================================================================

void RearLightController::UpdatePoliceTurboEffect() noexcept
{

    constexpr std::uint32_t IntervalMs = 100;

    const std::uint32_t now = millis();

    if ((now - m_policeTurboLastUpdate) <
        IntervalMs)
    {
        return;
    }

    m_policeTurboLastUpdate = now;

    //=========================================================================
    // Alternar
    //=========================================================================

    m_policeTurboState =
        !m_policeTurboState;

    //=========================================================================
    // LED 1 = Azul
    // LED 2 = OFF
    //=========================================================================

    if (m_policeTurboState)
    {
        SetLedColor(
            0,
            0,
            0,
            255);

        SetLedColor(
            1,
            0,
            0,
            0);
    }

    //=========================================================================
    // LED 1 = OFF
    // LED 2 = Rojo
    //=========================================================================

    else
    {
        SetLedColor(
            0,
            0,
            0,
            0);

        SetLedColor(
            1,
            255,
            0,
            0);
    }
}

//=============================================================================
// Is Police Profile
//=============================================================================

bool RearLightController::IsPoliceProfile(
    const VehicleProfiles::DrivingProfile& profile) const noexcept
{

    return std::strcmp(
        profile.name,
        "Police") == 0;
}

//=============================================================================
// Get Profile Color
//=============================================================================

void RearLightController::GetProfileColor(
    const VehicleProfiles::DrivingProfile& profile,
    std::uint8_t& red,
    std::uint8_t& green,
    std::uint8_t& blue) const noexcept
{
    //=========================================================================
    // Rookie = Verde
    //=========================================================================

    if (std::strcmp(
            profile.name,
            "Rookie") == 0)
    {
        red = 0;
        green = 255;
        blue = 0;

        return;
    }

    //=========================================================================
    // Normal = Azul
    //=========================================================================

    if (std::strcmp(
            profile.name,
            "Normal") == 0)
    {
        red = 0;
        green = 0;
        blue = 255;

        return;
    }

    //=========================================================================
    // Advanced = Naranja
    //=========================================================================

    if (std::strcmp(
            profile.name,
            "Advanced") == 0)
    {
        red = 255;
        green = 80;
        blue = 0;

        return;
    }

    //=========================================================================
    // Drift = Violeta
    //=========================================================================

    if (std::strcmp(
            profile.name,
            "Drift") == 0)
    {
        red = 180;
        green = 0;
        blue = 255;

        return;
    }

    //=========================================================================
    // Police = Azul
    //
    // Solo representa el color base durante startup.
    // El efecto Police utiliza azul/rojo.
    //=========================================================================

    if (std::strcmp(
            profile.name,
            "Police") == 0)
    {
        red = 0;
        green = 0;
        blue = 255;

        return;
    }

    //=========================================================================
    // Fallback
    //=========================================================================

    red = 0;
    green = 0;
    blue = 0;
}

//=============================================================================
// Set Color - ambos LEDs
//=============================================================================

void RearLightController::SetColor(
    std::uint8_t red,
    std::uint8_t green,
    std::uint8_t blue) noexcept
{
    for (std::uint8_t i = 0;i < LedCount;++i)
    {
        m_strip.setPixelColor(
            i,
            m_strip.Color(
                red,
                green,
                blue));
    }

    m_strip.show();
}

//=============================================================================
// Set LED Color - LED individual
//=============================================================================

void RearLightController::SetLedColor(
    std::uint8_t index,
    std::uint8_t red,
    std::uint8_t green,
    std::uint8_t blue) noexcept
{
    if (index >= LedCount)
    {
        return;
    }

    m_strip.setPixelColor(
        index,
        m_strip.Color(
            red,
            green,
            blue));

    m_strip.show();
}

//=============================================================================
// White
//=============================================================================

void RearLightController::SetWhite() noexcept
{
    SetColor(
        255,
        255,
        255);
}

//=============================================================================
// Clear
//=============================================================================

void RearLightController::Clear() noexcept
{
    m_strip.clear();

    m_strip.show();
}

} // namespace MK