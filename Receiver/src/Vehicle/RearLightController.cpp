/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : RearLightController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador de las luces traseras del Kart.
 *
 * Gestiona:
 * - Animación de inicio.
 * - Cambio de perfil.
 * - Luz blanca durante reversa.
 * - Efecto Turbo.
 ******************************************************************************/

#include "RearLightController.h"

#include <Arduino.h>

#include <cstring>

namespace MK
{

//=============================================================================
// Constructor
//=============================================================================

RearLightController::RearLightController()
    : m_strip(
          LedCount,
          LedPin,
          NEO_GRB + NEO_KHZ800)
{
}

//=============================================================================
// Ciclo de vida
//=============================================================================

bool RearLightController::Begin() noexcept
{
    //---------------------------------------------------------------------
    // Inicializar NeoPixel
    //---------------------------------------------------------------------

    m_strip.begin();

    m_strip.setBrightness(
        Brightness);

    //---------------------------------------------------------------------
    // Estado inicial
    //---------------------------------------------------------------------

    Clear();

    m_hasLastCommand = false;

    //---------------------------------------------------------------------
    // Perfil
    //---------------------------------------------------------------------

    m_lastProfileName = nullptr;

    //---------------------------------------------------------------------
    // Startup
    //---------------------------------------------------------------------

    m_startupAnimationActive = true;

    m_startupBlinkCount = 0;

    m_startupBlinkOn = false;

    m_startupBlinkLastUpdate = millis();

    //---------------------------------------------------------------------
    // Perfil
    //---------------------------------------------------------------------

    m_profileAnimationActive = false;

    m_profileBlinkCount = 0;

    m_profileBlinkOn = false;

    m_profileBlinkLastUpdate = 0;

    //---------------------------------------------------------------------
    // Turbo
    //---------------------------------------------------------------------

    m_turboActive = false;

    m_turboLastUpdate = 0;

    m_turboPhase = 0;

    return true;
}

//=============================================================================
// Update
//=============================================================================

void RearLightController::Update(
    const Protocol::DriverCommand& command,
    const VehicleProfiles::DrivingProfile& profile) noexcept
{
    using Types::Vehicle::Direction;
    using Types::Vehicle::Turbo;

    //---------------------------------------------------------------------
    // Detectar cambio de perfil
    //---------------------------------------------------------------------

    const bool profileChanged =
        m_lastProfileName == nullptr ||
        std::strcmp(
            m_lastProfileName,
            profile.name) != 0;

    if (profileChanged)
    {
        m_lastProfileName =
            profile.name;

        //-----------------------------------------------------------------
        // Si todavía estamos arrancando, no iniciar otra animación.
        //-----------------------------------------------------------------

        if (!m_startupAnimationActive)
        {
            StartProfileAnimation(profile);
        }
    }

    //---------------------------------------------------------------------
    // Startup
    //
    // Tiene prioridad al iniciar el sistema.
    //---------------------------------------------------------------------

    if (m_startupAnimationActive)
    {
        UpdateStartupAnimation(profile);

        return;
    }

    //---------------------------------------------------------------------
    // Turbo
    //
    // Turbo tiene prioridad sobre reversa y perfil.
    //---------------------------------------------------------------------

    const bool turbo =
        command.turbo == Turbo::Enabled;

    if (turbo)
    {
        //-----------------------------------------------------------------
        // Entrada a Turbo
        //-----------------------------------------------------------------

        if (!m_turboActive)
        {
            m_turboActive = true;

            m_turboPhase = 0;

            m_turboLastUpdate =
                millis();
        }

        UpdateTurboEffect();

        return;
    }

    //---------------------------------------------------------------------
    // Salida de Turbo
    //---------------------------------------------------------------------

    if (m_turboActive)
    {
        m_turboActive = false;

        m_turboPhase = 0;

        Clear();
    }

    //---------------------------------------------------------------------
    // Reversa
    //---------------------------------------------------------------------

    if (command.direction == Direction::Reverse)
    {
        SetWhite();

        return;
    }

    //---------------------------------------------------------------------
    // Animación de cambio de perfil
    //---------------------------------------------------------------------

    if (m_profileAnimationActive)
    {
        UpdateProfileAnimation(profile);

        return;
    }

    //---------------------------------------------------------------------
    // Estado normal
    //
    // Las luces permanecen apagadas.
    //---------------------------------------------------------------------

    Clear();
}

//=============================================================================
// Stop
//=============================================================================

void RearLightController::Stop() noexcept
{
    m_turboActive = false;

    m_turboPhase = 0;

    m_startupAnimationActive = false;

    m_profileAnimationActive = false;

    Clear();
}

//=============================================================================
// Startup Animation
//=============================================================================

void RearLightController::StartStartupAnimation() noexcept
{
    m_startupAnimationActive = true;

    m_startupBlinkCount = 0;

    m_startupBlinkOn = false;

    m_startupBlinkLastUpdate = millis();
}

//=============================================================================

void RearLightController::UpdateStartupAnimation(
    const VehicleProfiles::DrivingProfile& profile) noexcept
{
    //---------------------------------------------------------------------
    // Tiempo entre estados ON/OFF
    //---------------------------------------------------------------------

    constexpr std::uint32_t BlinkIntervalMs = 180;

    const std::uint32_t now =
        millis();

    if ((now - m_startupBlinkLastUpdate) <
        BlinkIntervalMs)
    {
        return;
    }

    m_startupBlinkLastUpdate = now;

    //---------------------------------------------------------------------
    // Cambiar estado
    //---------------------------------------------------------------------

    m_startupBlinkOn =
        !m_startupBlinkOn;

    //---------------------------------------------------------------------
    // Encender
    //---------------------------------------------------------------------

    if (m_startupBlinkOn)
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

        return;
    }

    //---------------------------------------------------------------------
    // Apagar
    //---------------------------------------------------------------------

    Clear();

    ++m_startupBlinkCount;

    //---------------------------------------------------------------------
    // Dos BLINKS
    //---------------------------------------------------------------------

    if (m_startupBlinkCount >= 2)
    {
        m_startupAnimationActive = false;

        m_startupBlinkCount = 0;

        m_startupBlinkOn = false;

        Clear();
    }
}

//=============================================================================
// Profile Animation
//=============================================================================

void RearLightController::StartProfileAnimation(
    const VehicleProfiles::DrivingProfile& profile) noexcept
{
    m_profileAnimationActive = true;

    m_profileBlinkCount = 0;

    m_profileBlinkOn = false;

    m_profileBlinkLastUpdate = millis();

    //---------------------------------------------------------------------
    // Evitar warning por parámetro no utilizado en esta función.
    //---------------------------------------------------------------------

    (void)profile;
}

//=============================================================================

void RearLightController::UpdateProfileAnimation(
    const VehicleProfiles::DrivingProfile& profile) noexcept
{
    //---------------------------------------------------------------------
    // Tiempo entre ON/OFF
    //---------------------------------------------------------------------

    constexpr std::uint32_t BlinkIntervalMs = 180;

    const std::uint32_t now =
        millis();

    if ((now - m_profileBlinkLastUpdate) <
        BlinkIntervalMs)
    {
        return;
    }

    m_profileBlinkLastUpdate = now;

    //---------------------------------------------------------------------
    // Cambiar estado
    //---------------------------------------------------------------------

    m_profileBlinkOn =
        !m_profileBlinkOn;

    //---------------------------------------------------------------------
    // Encender
    //---------------------------------------------------------------------

    if (m_profileBlinkOn)
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

        return;
    }

    //---------------------------------------------------------------------
    // Apagar
    //---------------------------------------------------------------------

    Clear();

    ++m_profileBlinkCount;

    //---------------------------------------------------------------------
    // Tres BLINKS
    //---------------------------------------------------------------------

    if (m_profileBlinkCount >= 3)
    {
        m_profileAnimationActive = false;

        m_profileBlinkCount = 0;

        m_profileBlinkOn = false;

        Clear();
    }
}

//=============================================================================
// Turbo Effect
//=============================================================================

void RearLightController::UpdateTurboEffect() noexcept
{
    //=====================================================================
    // Velocidad del efecto
    //=====================================================================

    constexpr std::uint32_t FrameIntervalMs = 3;

    const std::uint32_t now =
        millis();

    if ((now - m_turboLastUpdate) <
        FrameIntervalMs)
    {
        return;
    }

    m_turboLastUpdate = now;

    //=====================================================================
    // Avanzar fase
    //=====================================================================

    m_turboPhase=m_turboPhase + 5;

    if (m_turboPhase >= 510)
    {
        m_turboPhase = 0;
    }

    //=====================================================================
    // Calcular color
    //=====================================================================

    std::uint8_t red = 0;
    std::uint8_t green = 0;
    std::uint8_t blue = 0;

    //---------------------------------------------------------------------
    // Naranja → Rojo
    //---------------------------------------------------------------------

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

    //---------------------------------------------------------------------
    // Rojo → Morado
    //---------------------------------------------------------------------

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

    //---------------------------------------------------------------------
    // Morado → Azul
    //---------------------------------------------------------------------

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

    //---------------------------------------------------------------------
    // Azul → Morado → Naranja
    //---------------------------------------------------------------------

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

    //=====================================================================
    // Brillo dinámico
    //=====================================================================
    //
    // El brillo sube y baja durante el ciclo para dar sensación
    // de llama / energía.
    //
    // 0   → brillo mínimo
    // 255 → brillo máximo
    //
    // Usamos una onda triangular para que el efecto sea suave.
    //=====================================================================

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

    //=====================================================================
    // Aplicar brillo al RGB
    //=====================================================================

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

    //=====================================================================
    // Aplicar a ambos LEDs
    //=====================================================================

    for (std::uint8_t i = 0;
         i < LedCount;
         ++i)
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
// Profile Color
//=============================================================================

void RearLightController::GetProfileColor(
    const VehicleProfiles::DrivingProfile& profile,
    std::uint8_t& red,
    std::uint8_t& green,
    std::uint8_t& blue) const noexcept
{
    //---------------------------------------------------------------------
    // Valores por defecto
    //---------------------------------------------------------------------

    red = 0;
    green = 0;
    blue = 0;

    //---------------------------------------------------------------------
    // Rookie → Verde
    //---------------------------------------------------------------------

    if (std::strcmp(
            profile.name,
            "Rookie") == 0)
    {
        green = 255;

        return;
    }

    //---------------------------------------------------------------------
    // Normal → Azul
    //---------------------------------------------------------------------

    if (std::strcmp(
            profile.name,
            "Normal") == 0)
    {
        blue = 255;

        return;
    }

    //---------------------------------------------------------------------
    // Advanced → Naranja
    //---------------------------------------------------------------------

    if (std::strcmp(
            profile.name,
            "Advanced") == 0)
    {
        red = 255;
        green = 80;

        return;
    }

    //---------------------------------------------------------------------
    // Drift → Violeta
    //---------------------------------------------------------------------

    if (std::strcmp(
            profile.name,
            "Drift") == 0)
    {
        red = 180;
        blue = 255;

        return;
    }
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
// Color
//=============================================================================

void RearLightController::SetColor(
    std::uint8_t red,
    std::uint8_t green,
    std::uint8_t blue) noexcept
{
    for (std::uint8_t i = 0;
         i < LedCount;
         ++i)
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
// Clear
//=============================================================================

void RearLightController::Clear() noexcept
{
    m_strip.clear();

    m_strip.show();
}

} // namespace MK