/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : AmbientLightController.cpp
 *
 * Ambiente:
 *   4 x WS2812B
 *   GPIO 19
 *
 * Modo NORMAL:
 *   Cambio de perfil
 *       ↓
 *   Color del perfil
 *       ↓
 *   10 segundos
 *       ↓
 *   Rainbow Shift
 *
 * Modo WTF:
 *
 *   Perfil + Turbo durante 2 segundos
 *       ↓
 *   Entra / sale de WTF
 *
 *   Perfil + Adelante/Atrás
 *       → cambia COLOR
 *
 *   Perfil + Derecha/Izquierda
 *       → cambia EFECTO
 *
 * Todo es NO BLOQUEANTE.
 ******************************************************************************/

#include "AmbientLightController.h"

namespace MK
{

//=============================================================================
// Begin
//=============================================================================

bool AmbientLightController::Begin() noexcept
{
    m_strip.begin();

    m_strip.setBrightness(Brightness);

    // Inicialmente apagamos todos los LEDs.
    SetAll(m_strip.Color(0, 0, 0));

    m_started = true;
    m_enabled = true;

    m_wtfMode = false;
    m_rainbowActive = false;

    m_profileStartedAt = millis();
    m_lastRainbowUpdate = m_profileStartedAt;

    m_rainbowOffset = 0;

    m_currentColor = AmbientColor::Blue;
    m_currentEffect = AmbientEffect::Solid;

    return true;
}

//=============================================================================
// Update
//=============================================================================

void AmbientLightController::Update() noexcept
{
    if (!m_started || !m_enabled)
    {
        return;
    }

    const std::uint32_t now = millis();

    //=====================================================================
    // WTF MODE
    //=====================================================================

    if (m_wtfMode)
    {
        ShowWtf();

        return;
    }

    //=====================================================================
    // MODO NORMAL
    //=====================================================================

    // Mientras no hayan pasado los 10 segundos,
    // mantenemos el color correspondiente al perfil.
    if (!m_rainbowActive)
    {
        if ((now - m_profileStartedAt) >= ProfileColorTimeMs)
        {
            m_rainbowActive = true;

            m_rainbowOffset = 0;

            m_lastRainbowUpdate = now;

            ShowRainbow();
        }

        return;
    }

    //=====================================================================
    // RAINBOW
    //=====================================================================

    if ((now - m_lastRainbowUpdate) >= RainbowStepMs)
    {
        m_lastRainbowUpdate = now;

        ++m_rainbowOffset;

        ShowRainbow();
    }
}

//=============================================================================
// SetProfile
//=============================================================================

void AmbientLightController::SetProfile(
    VehicleProfiles::DrivingProfileId profile) noexcept
{
    m_profile = profile;

    //=====================================================================
    // Si estábamos en WTF, cambiar de perfil cancela WTF.
    //=====================================================================

    if (m_wtfMode)
    {
        m_wtfMode = false;

        m_currentColor = AmbientColor::Blue;
        m_currentEffect = AmbientEffect::Solid;
    }

    if (!m_started)
    {
        return;
    }

    if (!m_enabled)
    {
        return;
    }

    //=====================================================================
    // Reiniciamos la secuencia:
    //
    // Perfil
    //   ↓
    // Color
    //   ↓
    // 10 segundos
    //   ↓
    // Rainbow
    //=====================================================================

    m_rainbowActive = false;

    m_profileStartedAt = millis();

    m_lastRainbowUpdate = m_profileStartedAt;

    m_rainbowOffset = 0;

    ShowProfileColor();
}

//=============================================================================
// WTF MODE
//=============================================================================

void AmbientLightController::ToggleWtfMode() noexcept
{
    if (!m_started)
    {
        return;
    }

    m_wtfMode = !m_wtfMode;

    // Reiniciamos la animación del efecto.
    m_rainbowOffset = 0;

    m_lastRainbowUpdate = millis();

    //=====================================================================
    // ENTRAR A WTF
    //=====================================================================

    if (m_wtfMode)
    {
        // Primer WTF:
        //
        // Azul + Fijo
        //
        m_currentColor = AmbientColor::Blue;

        m_currentEffect = AmbientEffect::Solid;

        ShowCurrentEffect();

        return;
    }

    //=====================================================================
    // SALIR DE WTF
    //=====================================================================

    // Al salir:
    //
    // Color del perfil
    //      ↓
    //    10 s
    //      ↓
    //   Rainbow
    //

    m_rainbowActive = false;

    m_profileStartedAt = millis();

    m_lastRainbowUpdate = m_profileStartedAt;

    m_rainbowOffset = 0;

    ShowProfileColor();
}

//=============================================================================
// IsWtfMode
//=============================================================================

bool AmbientLightController::IsWtfMode() const noexcept
{
    return m_wtfMode;
}

//=============================================================================
// NextColor
//=============================================================================

void AmbientLightController::NextColor() noexcept
{
    if (!m_wtfMode)
    {
        return;
    }

    const std::uint8_t current =
        static_cast<std::uint8_t>(m_currentColor);

    std::uint8_t next =
        current + 1;

    if (next >=
        static_cast<std::uint8_t>(AmbientColor::Count))
    {
        next = 0;
    }

    m_currentColor =
        static_cast<AmbientColor>(next);

    // Si seleccionamos OFF, simplemente apagamos.
    if (m_currentColor == AmbientColor::Off)
    {
        SetAll(m_strip.Color(0, 0, 0));

        return;
    }

    // Police es un efecto especial.
    if (m_currentColor == AmbientColor::Police)
    {
        ShowPolice();

        return;
    }

    ShowCurrentEffect();
}

//=============================================================================
// PreviousColor
//=============================================================================

void AmbientLightController::PreviousColor() noexcept
{
    if (!m_wtfMode)
    {
        return;
    }

    const std::uint8_t current =
        static_cast<std::uint8_t>(m_currentColor);

    std::uint8_t previous;

    if (current == 0)
    {
        previous =
            static_cast<std::uint8_t>(
                AmbientColor::Count) - 1;
    }
    else
    {
        previous = current - 1;
    }

    m_currentColor =
        static_cast<AmbientColor>(previous);

    //=====================================================================
    // OFF
    //=====================================================================

    if (m_currentColor == AmbientColor::Off)
    {
        SetAll(m_strip.Color(0, 0, 0));

        return;
    }

    //=====================================================================
    // POLICE
    //=====================================================================

    if (m_currentColor == AmbientColor::Police)
    {
        ShowPolice();

        return;
    }

    ShowCurrentEffect();
}

//=============================================================================
// NextEffect
//=============================================================================

void AmbientLightController::NextEffect() noexcept
{
    if (!m_wtfMode)
    {
        return;
    }

    // OFF no tiene efectos.
    if (m_currentColor == AmbientColor::Off)
    {
        return;
    }

    // Police tampoco utiliza los efectos normales.
    if (m_currentColor == AmbientColor::Police)
    {
        return;
    }

    const std::uint8_t current =
        static_cast<std::uint8_t>(m_currentEffect);

    std::uint8_t next =
        current + 1;

    if (next >=
        static_cast<std::uint8_t>(AmbientEffect::Count))
    {
        next = 0;
    }

    m_currentEffect =
        static_cast<AmbientEffect>(next);

    m_rainbowOffset = 0;

    ShowCurrentEffect();
}

//=============================================================================
// PreviousEffect
//=============================================================================

void AmbientLightController::PreviousEffect() noexcept
{
    if (!m_wtfMode)
    {
        return;
    }

    if (m_currentColor == AmbientColor::Off)
    {
        return;
    }

    if (m_currentColor == AmbientColor::Police)
    {
        return;
    }

    const std::uint8_t current =
        static_cast<std::uint8_t>(m_currentEffect);

    std::uint8_t previous;

    if (current == 0)
    {
        previous =
            static_cast<std::uint8_t>(
                AmbientEffect::Count) - 1;
    }
    else
    {
        previous = current - 1;
    }

    m_currentEffect =
        static_cast<AmbientEffect>(previous);

    m_rainbowOffset = 0;

    ShowCurrentEffect();
}

//=============================================================================
// CurrentColor
//=============================================================================

AmbientLightController::AmbientColor
AmbientLightController::CurrentColor() const noexcept
{
    return m_currentColor;
}

//=============================================================================
// CurrentEffect
//=============================================================================

AmbientLightController::AmbientEffect
AmbientLightController::CurrentEffect() const noexcept
{
    return m_currentEffect;
}

//=============================================================================
// SetEnabled
//=============================================================================

void AmbientLightController::SetEnabled(
    bool enabled) noexcept
{
    m_enabled = enabled;

    if (!m_enabled)
    {
        SetAll(m_strip.Color(0, 0, 0));
    }
    else
    {
        m_rainbowActive = false;

        m_profileStartedAt = millis();

        ShowProfileColor();
    }
}

//=============================================================================
// IsEnabled
//=============================================================================

bool AmbientLightController::IsEnabled() const noexcept
{
    return m_enabled;
}

//=============================================================================
// Off
//=============================================================================

void AmbientLightController::Off() noexcept
{
    m_enabled = false;

    m_wtfMode = false;

    m_rainbowActive = false;

    SetAll(m_strip.Color(0, 0, 0));
}

//=============================================================================
// ColorForProfile
//=============================================================================

std::uint32_t AmbientLightController::ColorForProfile(
    VehicleProfiles::DrivingProfileId profile) const noexcept
{
    switch (profile)
    {
        case VehicleProfiles::DrivingProfileId::Rookie:
            return m_strip.Color(0, 255, 0);

        case VehicleProfiles::DrivingProfileId::Normal:
            return m_strip.Color(0, 0, 255);

        case VehicleProfiles::DrivingProfileId::Advanced:
            return m_strip.Color(255, 80, 0);

        case VehicleProfiles::DrivingProfileId::Drift:
            return m_strip.Color(180, 0, 255);

        case VehicleProfiles::DrivingProfileId::Police:
            return m_strip.Color(0, 80, 255);

        default:
            return m_strip.Color(255, 255, 255);
    }
}

//=============================================================================
// ColorForAmbient
//=============================================================================

std::uint32_t AmbientLightController::ColorForAmbient(
    AmbientColor color) const noexcept
{
    switch (color)
    {
        case AmbientColor::Blue:
            return m_strip.Color(
                0,
                0,
                255);

        case AmbientColor::Cyan:
            return m_strip.Color(
                0,
                220,
                255);

        case AmbientColor::White:
            return m_strip.Color(
                255,
                255,
                255);

        case AmbientColor::Red:
            return m_strip.Color(
                255,
                0,
                0);

        case AmbientColor::Orange:
            return m_strip.Color(
                255,
                80,
                0);

        case AmbientColor::Yellow:
            return m_strip.Color(
                255,
                220,
                0);

        case AmbientColor::Green:
            return m_strip.Color(
                0,
                255,
                0);

        case AmbientColor::Purple:
            return m_strip.Color(
                180,
                0,
                255);

        case AmbientColor::Police:
        case AmbientColor::Off:
        default:
            return m_strip.Color(
                0,
                0,
                0);
    }
}

//=============================================================================
// ShowProfileColor
//=============================================================================

void AmbientLightController::ShowProfileColor() noexcept
{
    SetAll(
        ColorForProfile(m_profile));
}

//=============================================================================
// ShowCurrentColor
//=============================================================================

void AmbientLightController::ShowCurrentColor() noexcept
{
    if (m_currentColor == AmbientColor::Off)
    {
        SetAll(
            m_strip.Color(0, 0, 0));

        return;
    }

    if (m_currentColor == AmbientColor::Police)
    {
        ShowPolice();

        return;
    }

    SetAll(
        ColorForAmbient(m_currentColor));
}

//=============================================================================
// ShowCurrentEffect
//=============================================================================

void AmbientLightController::ShowCurrentEffect() noexcept
{
    if (!m_wtfMode)
    {
        return;
    }

    if (m_currentColor == AmbientColor::Off)
    {
        SetAll(
            m_strip.Color(0, 0, 0));

        return;
    }

    if (m_currentColor == AmbientColor::Police)
    {
        ShowPolice();

        return;
    }

    ShowWtf();
}

//=============================================================================
// ShowWtf
//=============================================================================

void AmbientLightController::ShowWtf() noexcept
{
    switch (m_currentEffect)
    {
        case AmbientEffect::Solid:
            ShowSolid();
            break;

        case AmbientEffect::Fade:
            ShowFade();
            break;

        case AmbientEffect::SequentialFade:
            ShowSequentialFade();
            break;

        case AmbientEffect::AlternateFade:
            ShowAlternateFade();
            break;

        case AmbientEffect::FastBlink:
            ShowFastBlink();
            break;

        case AmbientEffect::SlowBlink:
            ShowSlowBlink();
            break;

        case AmbientEffect::Pulse:
            ShowPulse();
            break;

        default:
            ShowSolid();
            break;
    }
}

//=============================================================================
// SOLID
//=============================================================================

void AmbientLightController::ShowSolid() noexcept
{
    ShowCurrentColor();
}

//=============================================================================
// FADE PAREJO
//=============================================================================

void AmbientLightController::ShowFade() noexcept
{
    const std::uint32_t now = millis();

    constexpr std::uint32_t FadePeriodMs = 1800;

    const std::uint32_t elapsed =
        now % FadePeriodMs;

    std::uint8_t brightness;

    if (elapsed < FadePeriodMs / 2)
    {
        brightness =
            static_cast<std::uint8_t>(
                (elapsed * 255) /
                (FadePeriodMs / 2));
    }
    else
    {
        brightness =
            static_cast<std::uint8_t>(
                255 -
                ((elapsed - (FadePeriodMs / 2)) * 255) /
                (FadePeriodMs / 2));
    }

    const std::uint32_t color =
        ColorForAmbient(m_currentColor);

    const std::uint8_t r =
        static_cast<std::uint8_t>(
            ((color >> 16) & 0xFF) *
            brightness / 255);

    const std::uint8_t g =
        static_cast<std::uint8_t>(
            ((color >> 8) & 0xFF) *
            brightness / 255);

    const std::uint8_t b =
        static_cast<std::uint8_t>(
            (color & 0xFF) *
            brightness / 255);

    for (std::uint8_t i = 0;
         i < LedCount;
         ++i)
    {
        m_strip.setPixelColor(
            i,
            r,
            g,
            b);
    }

    m_strip.show();
}

//=============================================================================
// FADE SECUENCIAL
//=============================================================================

void AmbientLightController::ShowSequentialFade() noexcept
{
    const std::uint32_t now = millis();

    constexpr std::uint32_t LedPeriodMs = 900;

    const std::uint32_t color =
        ColorForAmbient(m_currentColor);

    const std::uint8_t r =
        static_cast<std::uint8_t>(
            (color >> 16) & 0xFF);

    const std::uint8_t g =
        static_cast<std::uint8_t>(
            (color >> 8) & 0xFF);

    const std::uint8_t b =
        static_cast<std::uint8_t>(
            color & 0xFF);

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const std::uint32_t phase =
            (now +
             (led * (LedPeriodMs / 4))) %
            LedPeriodMs;

        std::uint8_t brightness;

        if (phase < LedPeriodMs / 2)
        {
            brightness =
                static_cast<std::uint8_t>(
                    (phase * 255) /
                    (LedPeriodMs / 2));
        }
        else
        {
            brightness =
                static_cast<std::uint8_t>(
                    255 -
                    ((phase -
                      (LedPeriodMs / 2)) * 255) /
                    (LedPeriodMs / 2));
        }

        SetPixelScaled(
            led,
            r,
            g,
            b,
            brightness);
    }

    m_strip.show();
}

//=============================================================================
// FADE ALTERNO
//=============================================================================

void AmbientLightController::ShowAlternateFade() noexcept
{
    const std::uint32_t now = millis();

    constexpr std::uint32_t PeriodMs = 1200;

    const std::uint32_t phase =
        now % PeriodMs;

    std::uint8_t brightnessA;
    std::uint8_t brightnessB;

    if (phase < PeriodMs / 2)
    {
        brightnessA =
            static_cast<std::uint8_t>(
                (phase * 255) /
                (PeriodMs / 2));

        brightnessB =
            static_cast<std::uint8_t>(
                255 -
                brightnessA);
    }
    else
    {
        brightnessA =
            static_cast<std::uint8_t>(
                255 -
                ((phase -
                  (PeriodMs / 2)) * 255) /
                (PeriodMs / 2));

        brightnessB =
            static_cast<std::uint8_t>(
                255 -
                brightnessA);
    }

    const std::uint32_t color =
        ColorForAmbient(m_currentColor);

    const std::uint8_t r =
        static_cast<std::uint8_t>(
            (color >> 16) & 0xFF);

    const std::uint8_t g =
        static_cast<std::uint8_t>(
            (color >> 8) & 0xFF);

    const std::uint8_t b =
        static_cast<std::uint8_t>(
            color & 0xFF);

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const bool even =
            (led % 2) == 0;

        SetPixelScaled(
            led,
            r,
            g,
            b,
            even
                ? brightnessA
                : brightnessB);
    }

    m_strip.show();
}

//=============================================================================
// BLINK RAPIDO
//=============================================================================

void AmbientLightController::ShowFastBlink() noexcept
{
    constexpr std::uint32_t PeriodMs = 180;

    const bool on =
        ((millis() / PeriodMs) % 2) == 0;

    if (on)
    {
        ShowCurrentColor();
    }
    else
    {
        SetAll(
            m_strip.Color(0, 0, 0));
    }
}

//=============================================================================
// BLINK LENTO
//=============================================================================

void AmbientLightController::ShowSlowBlink() noexcept
{
    constexpr std::uint32_t PeriodMs = 700;

    const bool on =
        ((millis() / PeriodMs) % 2) == 0;

    if (on)
    {
        ShowCurrentColor();
    }
    else
    {
        SetAll(
            m_strip.Color(0, 0, 0));
    }
}

//=============================================================================
// PULSE
//=============================================================================

void AmbientLightController::ShowPulse() noexcept
{
    constexpr std::uint32_t PeriodMs = 1400;

    const std::uint32_t phase =
        millis() % PeriodMs;

    std::uint8_t brightness;

    if (phase < PeriodMs / 2)
    {
        brightness =
            static_cast<std::uint8_t>(
                40 +
                ((phase * 215) /
                 (PeriodMs / 2)));
    }
    else
    {
        brightness =
            static_cast<std::uint8_t>(
                255 -
                (((phase -
                   (PeriodMs / 2)) * 215) /
                 (PeriodMs / 2)));
    }

    const std::uint32_t color =
        ColorForAmbient(m_currentColor);

    const std::uint8_t r =
        static_cast<std::uint8_t>(
            (color >> 16) & 0xFF);

    const std::uint8_t g =
        static_cast<std::uint8_t>(
            (color >> 8) & 0xFF);

    const std::uint8_t b =
        static_cast<std::uint8_t>(
            color & 0xFF);

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        SetPixelScaled(
            led,
            r,
            g,
            b,
            brightness);
    }

    m_strip.show();
}

//=============================================================================
// POLICE
//
// Este efecto NO depende de AmbientEffect.
//
// Es un estado especial del selector de COLOR.
//
// Comportamiento:
//   Azul -> Rojo -> Azul -> Rojo
//
// Pares y nones se alternan lentamente.
//=============================================================================

void AmbientLightController::ShowPolice() noexcept
{
    constexpr std::uint32_t PolicePeriodMs = 1000;

    const bool state =
        ((millis() / PolicePeriodMs) % 2) == 0;

    const std::uint32_t blue =
        m_strip.Color(
            0,
            50,
            255);

    const std::uint32_t red =
        m_strip.Color(
            255,
            0,
            0);

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const bool even =
            (led % 2) == 0;

        if (state)
        {
            m_strip.setPixelColor(
                led,
                even
                    ? blue
                    : red);
        }
        else
        {
            m_strip.setPixelColor(
                led,
                even
                    ? red
                    : blue);
        }
    }

    m_strip.show();
}

//=============================================================================
// RAINBOW
//=============================================================================

void AmbientLightController::ShowRainbow() noexcept
{
    constexpr std::uint8_t SectionSize = 64;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const std::uint8_t position =
            static_cast<std::uint8_t>(
                m_rainbowOffset +
                (led * SectionSize));

        m_strip.setPixelColor(
            led,
            Wheel(position));
    }

    m_strip.show();
}

//=============================================================================
// Wheel
//=============================================================================

std::uint32_t AmbientLightController::Wheel(
    std::uint8_t position) const noexcept
{
    position =
        255 - position;

    if (position < 85)
    {
        return m_strip.Color(
            255 - position * 3,
            0,
            position * 3);
    }

    if (position < 170)
    {
        position -= 85;

        return m_strip.Color(
            0,
            position * 3,
            255 - position * 3);
    }

    position -= 170;

    return m_strip.Color(
        position * 3,
        255 - position * 3,
        0);
}

//=============================================================================
// SetAll
//=============================================================================

void AmbientLightController::SetAll(
    std::uint32_t color) noexcept
{
    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        m_strip.setPixelColor(
            led,
            color);
    }

    m_strip.show();
}

//=============================================================================
// SetPixelScaled
//=============================================================================

void AmbientLightController::SetPixelScaled(
    std::uint8_t led,
    std::uint8_t red,
    std::uint8_t green,
    std::uint8_t blue,
    std::uint8_t scale) noexcept
{
    const std::uint8_t r =
        static_cast<std::uint8_t>(
            (static_cast<std::uint16_t>(red) *
             scale) / 255);

    const std::uint8_t g =
        static_cast<std::uint8_t>(
            (static_cast<std::uint16_t>(green) *
             scale) / 255);

    const std::uint8_t b =
        static_cast<std::uint8_t>(
            (static_cast<std::uint16_t>(blue) *
             scale) / 255);

    m_strip.setPixelColor(
        led,
        m_strip.Color(
            r,
            g,
            b));
}

//=============================================================================
// TriangularWave
//=============================================================================

std::uint8_t AmbientLightController::TriangularWave(
    std::uint16_t phase) noexcept
{
    const std::uint8_t p =
        static_cast<std::uint8_t>(
            phase & 0xFF);

    if (p < 128)
    {
        return static_cast<std::uint8_t>(
            p * 2);
    }

    return static_cast<std::uint8_t>(
        255 -
        ((p - 128) * 2));
}

} // namespace MK