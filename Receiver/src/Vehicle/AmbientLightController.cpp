/******************************************************************************

* Proyecto : MarioKart ESP32 RC
* Archivo  : AmbientLightController.cpp
* Autor    : Narciso Ivan Cisneros Acosta
*
* Descripción:
* Implementación del controlador de iluminación ambiental.
*

******************************************************************************/

#include "AmbientLightController.h"

#include <algorithm>

namespace MK
{

bool AmbientLightController::Begin() noexcept
{
m_strip.begin();
m_strip.setBrightness(Brightness);
m_strip.clear();
m_strip.show();


m_enabled = true;
m_wtfMode = false;

m_currentColor = AmbientColor::Blue;
m_currentEffect = AmbientEffect::Solid;

m_savedWtfColor = AmbientColor::Blue;
m_savedWtfEffect = AmbientEffect::Solid;

m_profileDisplayActive = true;
m_profileStartedAt = millis();

m_rainbowOffset = 0;
m_lastRainbowUpdate = millis();

m_lastEffectUpdate = millis();
m_effectPhase = 0;
m_effectState = false;

m_effectBrightness = 0;
m_effectDirection = 1;

m_wtfConfirmationActive = false;
m_wtfConfirmationCount = 0;
m_wtfConfirmationState = false;
m_wtfConfirmationStartedAt = 0;

m_rainbowActive = false;

return true;


}

// -----------------------------------------------------------------------------
// Update
// -----------------------------------------------------------------------------

void AmbientLightController::Update() noexcept
{
if (!m_enabled)
{
return;
}


/*
 * Mientras se ejecuta la confirmación visual de WTF,
 * ésta tiene prioridad sobre cualquier otro efecto.
 */
if (m_wtfConfirmationActive)
{
    UpdateWtfConfirmationBlink();
    return;
}

if (m_wtfMode)
{
    UpdateWtfMode();
}
else if (m_customAmbientActive)
{
    /*
     * Ambiente personalizado configurado por el usuario.
     *
     * Permanece activo indefinidamente.
     */
    UpdateWtfMode();
}
else
{
    UpdateNormalMode();
}


}

// -----------------------------------------------------------------------------
// Normal mode
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateNormalMode() noexcept
{
const std::uint32_t now = millis();


/*
 * Durante los primeros 10 segundos mostramos el color del perfil.
 */
if (m_profileDisplayActive)
{
    ShowAmbientColor(
        m_currentColor);

    if ((now - m_profileStartedAt) >=
        ProfileDisplayTimeMs)
    {
        m_profileDisplayActive = false;
        m_rainbowActive = true;
        m_rainbowOffset = 0;
        m_lastRainbowUpdate = now;
    }

    return;
}

/*
 * Después de los 10 segundos:
 * Rainbow continuo.
 */
UpdateRainbow();


}

// -----------------------------------------------------------------------------
// WTF mode
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateWtfMode() noexcept
{
switch (m_currentColor)
{
case AmbientColor::Police:
UpdatePolice();
return;


    case AmbientColor::Rainbow:
        UpdateRainbow();
        return;

    case AmbientColor::Off:
        Off();
        return;

    default:
        break;
}

switch (m_currentEffect)
{
    case AmbientEffect::Solid:
        UpdateSolid();
        break;

    case AmbientEffect::Fade:
        UpdateFade();
        break;

    case AmbientEffect::SequentialFade:
        UpdateSequentialFade();
        break;

    case AmbientEffect::AlternateFade:
        UpdateAlternateFade();
        break;

    case AmbientEffect::FastBlink:
        UpdateFastBlink();
        break;

    case AmbientEffect::SlowBlink:
        UpdateSlowBlink();
        break;

    case AmbientEffect::Pulse:
        UpdatePulse();
        break;

    default:
        UpdateSolid();
        break;
}


}

// -----------------------------------------------------------------------------
// Profile
// -----------------------------------------------------------------------------

void AmbientLightController::SetProfile(
    VehicleProfiles::DrivingProfileId profile) noexcept
{
    m_profile = profile;

    /*
     * Un cambio de perfil siempre devuelve la iluminación
     * al comportamiento normal.
     */
    m_wtfMode = false;
    m_customAmbientActive = false;

    /*
     * IMPORTANTE:
     *
     * NO tocamos:
     *
     *   m_savedWtfColor
     *   m_savedWtfEffect
     *
     * La configuración WTF permanece guardada.
     */

    m_profileDisplayActive = true;
    m_profileStartedAt = millis();

    m_rainbowActive = false;
    m_rainbowOffset = 0;

    m_lastEffectUpdate = millis();
    m_effectPhase = 0;
    m_effectState = false;

    m_effectBrightness = 0;
    m_effectDirection = 1;

    /*
     * Color correspondiente al perfil.
     */
    switch (profile)
    {
        case VehicleProfiles::DrivingProfileId::Rookie:
            m_currentColor = AmbientColor::Green;
            break;

        case VehicleProfiles::DrivingProfileId::Normal:
            m_currentColor = AmbientColor::Blue;
            break;

        case VehicleProfiles::DrivingProfileId::Advanced:
            m_currentColor = AmbientColor::Orange;
            break;

        case VehicleProfiles::DrivingProfileId::Drift:
            m_currentColor = AmbientColor::Purple;
            break;

        case VehicleProfiles::DrivingProfileId::Police:
            m_currentColor = AmbientColor::Police;
            break;

        default:
            m_currentColor = AmbientColor::Blue;
            break;
    }

    /*
     * El efecto normal no importa mientras se muestra
     * el color del perfil.
     */
    ShowAmbientColor(m_currentColor);
}
// -----------------------------------------------------------------------------
// WTF toggle
// -----------------------------------------------------------------------------

void AmbientLightController::ToggleWtfMode() noexcept
{
    if (m_wtfMode)
    {
        /*
         * =========================================================
         * SALIENDO DE WTF
         * =========================================================
         *
         * Guardamos AMBAS cosas:
         *
         *   - color
         *   - efecto
         */
        m_savedWtfColor = m_currentColor;
        m_savedWtfEffect = m_currentEffect;

        /*
         * WTF deja de ser el modo de configuración.
         */
        m_wtfMode = false;

        /*
         * Pero el ambiente personalizado queda ACTIVO.
         *
         * Esta es la diferencia importante:
         *
         * WTF = configuración
         * CustomAmbient = ejecución permanente
         */
        m_customAmbientActive = true;

        /*
         * Ya no debe existir la secuencia:
         *
         * perfil → 10 segundos → Rainbow
         *
         * porque ya no estamos en modo normal.
         */
        m_profileDisplayActive = false;
        m_rainbowActive = false;

        /*
         * Reiniciamos el estado del efecto.
         */
        m_lastEffectUpdate = millis();
        m_effectPhase = 0;
        m_effectState = false;

        m_effectBrightness = 0;
        m_effectDirection = 1;

        /*
         * Confirmación visual de salida.
         */
        StartWtfConfirmationBlink();

        return;
    }

    /*
     * =========================================================
     * ENTRANDO EN WTF
     * =========================================================
     */

    /*
     * Recuperamos la última configuración guardada.
     */
    m_currentColor = m_savedWtfColor;
    m_currentEffect = m_savedWtfEffect;

    m_wtfMode = true;
    m_customAmbientActive = false;

    m_profileDisplayActive = false;
    m_rainbowActive = false;

    m_lastEffectUpdate = millis();
    m_effectPhase = 0;
    m_effectState = false;

    m_effectBrightness = 0;
    m_effectDirection = 1;

    /*
     * Confirmación visual.
     */
    StartWtfConfirmationBlink();
}
// -----------------------------------------------------------------------------
// WTF state
// -----------------------------------------------------------------------------

bool AmbientLightController::IsWtfMode() const noexcept
{
return m_wtfMode;
}

// -----------------------------------------------------------------------------
// Colors
// -----------------------------------------------------------------------------

void AmbientLightController::NextColor() noexcept
{
    if (!m_wtfMode)
    {
        return;
    }

    m_currentColor =
        NextNormalColor(m_currentColor);

    /*
     * Guardamos inmediatamente el color.
     */
    m_savedWtfColor = m_currentColor;

    /*
     * El efecto seleccionado NO cambia.
     */
    m_effectPhase = 0;
    m_effectState = false;
    m_effectBrightness = 0;
    m_effectDirection = 1;

    m_lastEffectUpdate = millis();
}

void AmbientLightController::PreviousColor() noexcept
{
    if (!m_wtfMode)
    {
        return;
    }

    m_currentColor =
        PreviousNormalColor(m_currentColor);

    /*
     * Guardamos inmediatamente el color.
     */
    m_savedWtfColor = m_currentColor;

    /*
     * El efecto seleccionado NO cambia.
     */
    m_effectPhase = 0;
    m_effectState = false;
    m_effectBrightness = 0;
    m_effectDirection = 1;

    m_lastEffectUpdate = millis();
}

void AmbientLightController::NextEffect() noexcept
{
    if (!m_wtfMode)
    {
        return;
    }

    /*
     * Police, Rainbow y Off son modos especiales de color.
     * No utilizan los efectos normales.
     */
    if (m_currentColor == AmbientColor::Police ||
        m_currentColor == AmbientColor::Rainbow ||
        m_currentColor == AmbientColor::Off)
    {
        return;
    }

    m_currentEffect =
        NextEffectValue(m_currentEffect);

    /*
     * Guardamos inmediatamente el efecto.
     *
     * El color permanece intacto.
     */
    m_savedWtfEffect = m_currentEffect;

    m_effectPhase = 0;
    m_effectState = false;
    m_effectBrightness = 0;
    m_effectDirection = 1;

    m_lastEffectUpdate = millis();
}

void AmbientLightController::PreviousEffect() noexcept
{
    if (!m_wtfMode)
    {
        return;
    }

    if (m_currentColor == AmbientColor::Police ||
        m_currentColor == AmbientColor::Rainbow ||
        m_currentColor == AmbientColor::Off)
    {
        return;
    }

    m_currentEffect =
        PreviousEffectValue(m_currentEffect);

    /*
     * Guardamos inmediatamente el efecto.
     *
     * El color permanece intacto.
     */
    m_savedWtfEffect = m_currentEffect;

    m_effectPhase = 0;
    m_effectState = false;
    m_effectBrightness = 0;
    m_effectDirection = 1;

    m_lastEffectUpdate = millis();
}

// -----------------------------------------------------------------------------
// Current configuration
// -----------------------------------------------------------------------------

AmbientLightController::AmbientColor
AmbientLightController::CurrentColor() const noexcept
{
return m_currentColor;
}

AmbientLightController::AmbientEffect
AmbientLightController::CurrentEffect() const noexcept
{
return m_currentEffect;
}

// -----------------------------------------------------------------------------
// Enable / disable
// -----------------------------------------------------------------------------

void AmbientLightController::Off() noexcept
{
m_strip.clear();
m_strip.show();
}

void AmbientLightController::SetEnabled(bool enabled) noexcept
{
m_enabled = enabled;


if (!m_enabled)
{
    Off();
}


}

bool AmbientLightController::IsEnabled() const noexcept
{
return m_enabled;
}

// -----------------------------------------------------------------------------
// WTF confirmation
// -----------------------------------------------------------------------------

void AmbientLightController::StartWtfConfirmationBlink() noexcept
{
m_wtfConfirmationActive = true;
m_wtfConfirmationCount = 0;
m_wtfConfirmationState = false;
m_wtfConfirmationStartedAt = millis();


/*
 * Comenzamos inmediatamente mostrando el color.
 * No mostramos OFF como primer estado visible.
 */
ShowCurrentConfiguration();


}

void AmbientLightController::UpdateWtfConfirmationBlink() noexcept
{
const std::uint32_t now = millis();


if ((now - m_wtfConfirmationStartedAt) <
    WtfConfirmationBlinkIntervalMs)
{
    return;
}

m_wtfConfirmationStartedAt = now;

m_wtfConfirmationState =
    !m_wtfConfirmationState;

if (m_wtfConfirmationState)
{
    ShowCurrentConfiguration();
}
else
{
    Off();

    ++m_wtfConfirmationCount;

    if (m_wtfConfirmationCount >=
        WtfConfirmationBlinkCount)
    {
        /*
         * Terminamos la confirmación.
         *
         * El siguiente Update() comenzará inmediatamente el
         * efecto seleccionado.
         */
        m_wtfConfirmationActive = false;
        m_wtfConfirmationState = false;

        m_lastEffectUpdate = millis();
        m_effectPhase = 0;
        m_effectState = false;
        m_effectBrightness = 0;
        m_effectDirection = 1;
    }
}


}

// -----------------------------------------------------------------------------
// Current configuration preview
// -----------------------------------------------------------------------------

void AmbientLightController::ShowCurrentConfiguration() noexcept
{
if (m_currentColor == AmbientColor::Police)
{
/*
* Para la confirmación mostramos azul como color representativo.
*/
ShowColor(0, 0, 255);
return;
}


if (m_currentColor == AmbientColor::Rainbow)
{
    ShowColor(255, 0, 0);
    return;
}

if (m_currentColor == AmbientColor::Off)
{
    Off();
    return;
}

const std::uint32_t rgb =
    ColorToRgb(m_currentColor);

const std::uint8_t red =
    static_cast<std::uint8_t>((rgb >> 16) & 0xFF);

const std::uint8_t green =
    static_cast<std::uint8_t>((rgb >> 8) & 0xFF);

const std::uint8_t blue =
    static_cast<std::uint8_t>(rgb & 0xFF);

ShowColor(red, green, blue);


}

// -----------------------------------------------------------------------------
// Solid
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateSolid() noexcept
{
ShowAmbientColor(m_currentColor);
}

// -----------------------------------------------------------------------------
// Fade
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateFade() noexcept
{
const std::uint32_t now = millis();


if ((now - m_lastEffectUpdate) < FadeStepMs)
{
    return;
}

m_lastEffectUpdate = now;

m_effectBrightness +=
    static_cast<std::int16_t>(m_effectDirection) * 5;

if (m_effectBrightness >= 255)
{
    m_effectBrightness = 255;
    m_effectDirection = -1;
}
else if (m_effectBrightness <= 20)
{
    m_effectBrightness = 20;
    m_effectDirection = 1;
}

const std::uint32_t rgb =
    ColorToRgb(m_currentColor);

const std::uint8_t red =
    static_cast<std::uint8_t>(
        ((rgb >> 16) & 0xFF) *
        m_effectBrightness / 255);

const std::uint8_t green =
    static_cast<std::uint8_t>(
        ((rgb >> 8) & 0xFF) *
        m_effectBrightness / 255);

const std::uint8_t blue =
    static_cast<std::uint8_t>(
        (rgb & 0xFF) *
        m_effectBrightness / 255);

ShowColor(red, green, blue);


}

// -----------------------------------------------------------------------------
// Sequential fade
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateSequentialFade() noexcept
{
const std::uint32_t now = millis();


if ((now - m_lastEffectUpdate) < FadeStepMs)
{
    return;
}

m_lastEffectUpdate = now;

const std::uint32_t rgb =
    ColorToRgb(m_currentColor);

const std::uint8_t baseRed =
    static_cast<std::uint8_t>((rgb >> 16) & 0xFF);

const std::uint8_t baseGreen =
    static_cast<std::uint8_t>((rgb >> 8) & 0xFF);

const std::uint8_t baseBlue =
    static_cast<std::uint8_t>(rgb & 0xFF);

for (std::uint8_t led = 0; led < LedCount; ++led)
{
    const std::uint8_t phase =
        static_cast<std::uint8_t>(
            m_effectPhase + led * 64);

    const std::uint8_t brightness =
        static_cast<std::uint8_t>(
            (std::sin(
                static_cast<float>(phase) *
                0.02454369f) + 1.0f) *
            127.5f);

    m_strip.setPixelColor(
        led,
        baseRed * brightness / 255,
        baseGreen * brightness / 255,
        baseBlue * brightness / 255);
}

m_strip.show();

++m_effectPhase;


}

// -----------------------------------------------------------------------------
// Alternate fade
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateAlternateFade() noexcept
{
const std::uint32_t now = millis();


if ((now - m_lastEffectUpdate) < FadeStepMs)
{
    return;
}

m_lastEffectUpdate = now;

m_effectBrightness +=
    static_cast<std::int16_t>(m_effectDirection) * 5;

if (m_effectBrightness >= 255)
{
    m_effectBrightness = 255;
    m_effectDirection = -1;
}
else if (m_effectBrightness <= 20)
{
    m_effectBrightness = 20;
    m_effectDirection = 1;
}

const std::uint32_t rgb =
    ColorToRgb(m_currentColor);

const std::uint8_t red =
    static_cast<std::uint8_t>((rgb >> 16) & 0xFF);

const std::uint8_t green =
    static_cast<std::uint8_t>((rgb >> 8) & 0xFF);

const std::uint8_t blue =
    static_cast<std::uint8_t>(rgb & 0xFF);

for (std::uint8_t led = 0; led < LedCount; ++led)
{
    const bool firstGroup =
        (led % 2) == 0;

    const bool visible =
        firstGroup == m_effectState;

    const std::uint8_t brightness =
        visible
            ? static_cast<std::uint8_t>(m_effectBrightness)
            : 0;

    m_strip.setPixelColor(
        led,
        red * brightness / 255,
        green * brightness / 255,
        blue * brightness / 255);
}

m_strip.show();

if (m_effectBrightness <= 20 ||
    m_effectBrightness >= 255)
{
    m_effectState = !m_effectState;
}


}

// -----------------------------------------------------------------------------
// Fast blink
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateFastBlink() noexcept
{
const std::uint32_t now = millis();


if ((now - m_lastEffectUpdate) <
    FastBlinkIntervalMs)
{
    return;
}

m_lastEffectUpdate = now;
m_effectState = !m_effectState;

if (m_effectState)
{
    ShowAmbientColor(m_currentColor);
}
else
{
    Off();
}


}

// -----------------------------------------------------------------------------
// Slow blink
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateSlowBlink() noexcept
{
const std::uint32_t now = millis();


if ((now - m_lastEffectUpdate) <
    SlowBlinkIntervalMs)
{
    return;
}

m_lastEffectUpdate = now;
m_effectState = !m_effectState;

if (m_effectState)
{
    ShowAmbientColor(m_currentColor);
}
else
{
    Off();
}


}

// -----------------------------------------------------------------------------
// Pulse
// -----------------------------------------------------------------------------

void AmbientLightController::UpdatePulse() noexcept
{
const std::uint32_t now = millis();


if ((now - m_lastEffectUpdate) < PulseStepMs)
{
    return;
}

m_lastEffectUpdate = now;

m_effectBrightness +=
    static_cast<std::int16_t>(m_effectDirection) * 8;

if (m_effectBrightness >= 255)
{
    m_effectBrightness = 255;
    m_effectDirection = -1;
}
else if (m_effectBrightness <= 20)
{
    m_effectBrightness = 20;
    m_effectDirection = 1;
}

const std::uint32_t rgb =
    ColorToRgb(m_currentColor);

const std::uint8_t red =
    static_cast<std::uint8_t>((rgb >> 16) & 0xFF);

const std::uint8_t green =
    static_cast<std::uint8_t>((rgb >> 8) & 0xFF);

const std::uint8_t blue =
    static_cast<std::uint8_t>(rgb & 0xFF);

ShowColor(
    red * m_effectBrightness / 255,
    green * m_effectBrightness / 255,
    blue * m_effectBrightness / 255);


}

// -----------------------------------------------------------------------------
// Police
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Police
// -----------------------------------------------------------------------------


void AmbientLightController::UpdatePolice() noexcept
{
    /*
     * =========================================================
     * POLICE
     * =========================================================
     *
     * Efecto de patrullaje alternado:
     *
     *      🔵 ⚫ 🔵 ⚫
     *      ⚫ 🔴 ⚫ 🔴
     *
     * Los LEDs pares e impares se alternan.
     *
     * La velocidad utiliza como referencia el mismo intervalo
     * definido para SlowBlink.
     *
     * IMPORTANTE:
     *
     * Police es un efecto independiente.
     * AlternateFade solamente sirve como referencia para la
     * distribución alternada de los LEDs.
     */

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        SlowBlinkIntervalMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    m_effectState = !m_effectState;

    for (std::uint8_t led = 0; led < LedCount; ++led)
    {
        const bool firstGroup =
            (led % 2) == 0;

        const bool visible =
            firstGroup == m_effectState;

        if (visible)
        {
            if (m_effectState)
            {
                /*
                 * Grupo de LEDs pares → Azul.
                 */
                m_strip.setPixelColor(
                    led,
                    0,
                    0,
                    255);
            }
            else
            {
                /*
                 * Grupo de LEDs impares → Rojo.
                 */
                m_strip.setPixelColor(
                    led,
                    255,
                    0,
                    0);
            }
        }
        else
        {
            /*
             * Grupo contrario apagado.
             */
            m_strip.setPixelColor(
                led,
                0,
                0,
                0);
        }
    }

    m_strip.show();
}


// -----------------------------------------------------------------------------
// Rainbow
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateRainbow() noexcept
{
const std::uint32_t now = millis();


if ((now - m_lastRainbowUpdate) <
    RainbowStepMs)
{
    return;
}

m_lastRainbowUpdate = now;

for (std::uint8_t led = 0; led < LedCount; ++led)
{
    const std::uint8_t position =
        static_cast<std::uint8_t>(
            m_rainbowOffset +
            led * 64);

    const std::uint32_t color =
        Wheel(position);

    m_strip.setPixelColor(led, color);
}

m_strip.show();

++m_rainbowOffset;


}

// -----------------------------------------------------------------------------
// Show helpers
// -----------------------------------------------------------------------------

void AmbientLightController::ShowColor(
std::uint8_t red,
std::uint8_t green,
std::uint8_t blue) noexcept
{
for (std::uint8_t led = 0; led < LedCount; ++led)
{
m_strip.setPixelColor(
led,
red,
green,
blue);
}


m_strip.show();


}

void AmbientLightController::ShowAmbientColor(
AmbientColor color) noexcept
{
if (color == AmbientColor::Police)
{
ShowColor(0, 0, 255);
return;
}


if (color == AmbientColor::Rainbow)
{
    UpdateRainbow();
    return;
}

if (color == AmbientColor::Off)
{
    Off();
    return;
}

const std::uint32_t rgb =
    ColorToRgb(color);

ShowColor(
    static_cast<std::uint8_t>((rgb >> 16) & 0xFF),
    static_cast<std::uint8_t>((rgb >> 8) & 0xFF),
    static_cast<std::uint8_t>(rgb & 0xFF));


}

// -----------------------------------------------------------------------------
// Color helpers
// -----------------------------------------------------------------------------

std::uint32_t AmbientLightController::ColorToRgb(
AmbientColor color) const noexcept
{
switch (color)
{
case AmbientColor::Blue:
return 0x0000FF;


    case AmbientColor::Cyan:
        return 0x00FFFF;

    case AmbientColor::White:
        return 0xFFFFFF;

    case AmbientColor::Red:
        return 0xFF0000;

    case AmbientColor::Orange:
        return 0xFF8000;

    case AmbientColor::Yellow:
        return 0xFFFF00;

    case AmbientColor::Green:
        return 0x00FF00;

    case AmbientColor::Purple:
        return 0xB000FF;

    default:
        return 0x000000;
}


}

AmbientLightController::AmbientColor
AmbientLightController::NextNormalColor(
    AmbientColor color) const noexcept
{
    constexpr std::uint8_t ColorCount =
        static_cast<std::uint8_t>(AmbientColor::Off) + 1;

    std::uint8_t index =
        static_cast<std::uint8_t>(color);

    ++index;

    if (index >= ColorCount)
    {
        index = 0;
    }

    return static_cast<AmbientColor>(index);
}


AmbientLightController::AmbientColor
AmbientLightController::PreviousNormalColor(
    AmbientColor color) const noexcept
{
    constexpr std::uint8_t ColorCount =
        static_cast<std::uint8_t>(AmbientColor::Off) + 1;

    std::uint8_t index =
        static_cast<std::uint8_t>(color);

    if (index == 0 ||
        index >= ColorCount)
    {
        index = ColorCount - 1;
    }
    else
    {
        --index;
    }

    return static_cast<AmbientColor>(index);
}


AmbientLightController::AmbientEffect
AmbientLightController::NextEffectValue(
    AmbientEffect effect) const noexcept
{
    std::uint8_t index =
        static_cast<std::uint8_t>(effect);

    ++index;

    if (index >=
        static_cast<std::uint8_t>(AmbientEffect::Count))
    {
        index = 0;
    }

    return static_cast<AmbientEffect>(index);
}

AmbientLightController::AmbientEffect
AmbientLightController::PreviousEffectValue(
    AmbientEffect effect) const noexcept
{
    std::uint8_t index =
        static_cast<std::uint8_t>(effect);

    if (index == 0)
    {
        index =
            static_cast<std::uint8_t>(
                AmbientEffect::Count) - 1;
    }
    else
    {
        --index;
    }

    return static_cast<AmbientEffect>(index);
}
// -----------------------------------------------------------------------------
// Rainbow wheel
// -----------------------------------------------------------------------------

std::uint32_t AmbientLightController::Wheel(
std::uint8_t position) const noexcept
{
position = 255 - position;


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

}
