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

    case AmbientEffect::Chase:
        UpdateChase();
        break;        

    case AmbientEffect::Scanner:
        UpdateScanner();
        break;    

    case AmbientEffect::Heartbeat:
        UpdateHeartbeat();
        break;

    case AmbientEffect::Sparkle:
        UpdateSparkle();
        break;

    case AmbientEffect::Lightning:
        UpdateLightning();
        break;

    case AmbientEffect::Fire:
        UpdateFire();
        break;

    case AmbientEffect::RainbowChase:
        UpdateRainbowChase();
        break;

    case AmbientEffect::RainbowWave:
        UpdateRainbowWave();
        break;

    case AmbientEffect::CenterOut:
        UpdateCenterOut();
        break;

    case AmbientEffect::Explosion:
        UpdateExplosion();
        break;

    case AmbientEffect::RainbowExplosion:
        UpdateRainbowExplosion();
        break;

    case AmbientEffect::CenterPulse:
        UpdateCenterPulse();
        break;

    case AmbientEffect::Ripple:
        UpdateRipple();
        break;

    case AmbientEffect::AlternatingWave:
        UpdateAlternatingWave();
        break;

    case AmbientEffect::Strobe:
        UpdateStrobe();
        break;

    case AmbientEffect::DoubleFlash:
        UpdateDoubleFlash();
        break;

    case AmbientEffect::Meteor:
        UpdateMeteor();
        break;

    case AmbientEffect::Breathing:
        UpdateBreathing();
        break;

    case AmbientEffect::Comet:
        UpdateComet();
        break;

    case AmbientEffect::Wave:
        UpdateWave();
        break;

    case AmbientEffect::Theater:
        UpdateTheater();
        break;

    case AmbientEffect::SideFade:
        UpdateSideFade();
        break;

    case AmbientEffect::MirrorWave:
        UpdateMirrorWave();
        break;

    case AmbientEffect::CenterSplit:
        UpdateCenterSplit();
        break;

    case AmbientEffect::DualChase:
        UpdateDualChase();
        break;

    case AmbientEffect::Crossing:
        UpdateCrossing();
        break;

    case AmbientEffect::SymmetricPulse:
        UpdateSymmetricPulse();
        break;

    case AmbientEffect::CenterPulseWave:
        UpdateCenterPulseWave();
        break;

    case AmbientEffect::SideChase:
        UpdateSideChase();
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
// Chase
// -----------------------------------------------------------------------------


void AmbientLightController::UpdateChase() noexcept
{
    /*
     * =========================================================
     * CHASE
     * =========================================================
     *
     * Un punto de luz recorre los LEDs siguiendo el orden
     * físico de la tira:
     *
     *      1 → 2 → 3 → 4 → 5 → 6 → ...
     *
     * El efecto utiliza LedCount, por lo que no depende
     * directamente de que existan 6 LEDs.
     *
     * La velocidad se controla mediante FadeStepMs.
     */

    static constexpr std::uint32_t ChaseStepMs = 100;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        ChaseStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    //-------------------------------------------------------------
    // Obtener el color seleccionado.
    //-------------------------------------------------------------

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    //-------------------------------------------------------------
    // Apagar todos los LEDs.
    //-------------------------------------------------------------

    m_strip.clear();

    //-------------------------------------------------------------
    // Encender únicamente el LED actual.
    //
    // m_effectPhase representa el índice del LED.
    //-------------------------------------------------------------

    m_strip.setPixelColor(
        m_effectPhase,
        red,
        green,
        blue);

    m_strip.show();

    //-------------------------------------------------------------
    // Siguiente LED.
    //-------------------------------------------------------------

    ++m_effectPhase;

    if (m_effectPhase >= LedCount)
    {
        m_effectPhase = 0;
    }
}

// -----------------------------------------------------------------------------
// Scanner
// -----------------------------------------------------------------------------


void AmbientLightController::UpdateScanner() noexcept
{
    /*
     * =========================================================
     * SCANNER
     * =========================================================
     *
     * Un punto de luz recorre los LEDs y regresa:
     *
     *      1 → 2 → 3 → 4 → 5 → 6
     *                      ↓
     *      1 ← 2 ← 3 ← 4 ← 5 ← 6
     *
     * En términos de índices:
     *
     *      0 → 1 → 2 → 3 → 4 → 5
     *                      ↓
     *      0 ← 1 ← 2 ← 3 ← 4 ← 5
     *
     * El efecto utiliza LedCount, por lo que no depende
     * directamente de que existan 6 LEDs.
     */

    static constexpr std::uint32_t ScannerStepMs = 100;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        ScannerStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    //-------------------------------------------------------------
    // Obtener el color seleccionado.
    //-------------------------------------------------------------

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    //-------------------------------------------------------------
    // Apagar todos los LEDs.
    //-------------------------------------------------------------

    m_strip.clear();

    //-------------------------------------------------------------
    // Encender únicamente el LED actual.
    //-------------------------------------------------------------

    m_strip.setPixelColor(
        m_effectPhase,
        red,
        green,
        blue);

    m_strip.show();

    //-------------------------------------------------------------
    // Avanzar o retroceder.
    //
    // m_effectDirection:
    //
    //      1  = hacia adelante
    //     -1  = hacia atrás
    //-------------------------------------------------------------

    if (m_effectDirection > 0)
    {
        //---------------------------------------------------------
        // 1 → 2 → 3 → 4 → 5 → 6
        //---------------------------------------------------------

        if (m_effectPhase >= (LedCount - 1))
        {
            m_effectDirection = -1;
            --m_effectPhase;
        }
        else
        {
            ++m_effectPhase;
        }
    }
    else
    {
        //---------------------------------------------------------
        // 6 → 5 → 4 → 3 → 2 → 1
        //---------------------------------------------------------

        if (m_effectPhase == 0)
        {
            m_effectDirection = 1;
            ++m_effectPhase;
        }
        else
        {
            --m_effectPhase;
        }
    }
}

// -----------------------------------------------------------------------------
// HEARTBEAT
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateHeartbeat() noexcept
{
    /*
     * =========================================================
     * HEARTBEAT
     * =========================================================
     *
     * Simula un latido:
     *
     *      💓  💓  .........  💓  💓  .........
     *
     * Secuencia:
     *
     *      Pulso fuerte
     *          ↓
     *      Pulso fuerte
     *          ↓
     *      Pausa
     *
     * El efecto se aplica a todos los LEDs al mismo tiempo.
     *
     * La intensidad cambia para dar la sensación de un
     * verdadero latido, diferenciándolo del efecto Pulse.
     */

    static constexpr std::uint32_t HeartbeatStepMs = 100;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        HeartbeatStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    /*
     * =========================================================
     * SECUENCIA DEL LATIDO
     * =========================================================
     *
     * m_effectPhase:
     *
     *      0 = reposo
     *      1 = subida del primer latido
     *      2 = bajada del primer latido
     *      3 = subida del segundo latido
     *      4 = bajada del segundo latido
     *      5 = pausa
     */

    std::uint8_t brightness = 0;

    switch (m_effectPhase)
    {
    case 0:
        brightness = 0;
        break;

    case 1:
        brightness = 255;
        break;

    case 2:
        brightness = 40;
        break;

    case 3:
        brightness = 255;
        break;

    case 4:
        brightness = 0;
        break;

    case 5:
        brightness = 0;
        break;

    default:
        m_effectPhase = 0;
        brightness = 0;
        break;
    }

    /*
     * Aplicamos la intensidad al color seleccionado.
     */

    const std::uint16_t scaledRed =
        (static_cast<std::uint16_t>(red) *
         brightness) / 255;

    const std::uint16_t scaledGreen =
        (static_cast<std::uint16_t>(green) *
         brightness) / 255;

    const std::uint16_t scaledBlue =
        (static_cast<std::uint16_t>(blue) *
         brightness) / 255;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        m_strip.setPixelColor(
            led,
            scaledRed,
            scaledGreen,
            scaledBlue);
    }

    m_strip.show();

    ++m_effectPhase;

    if (m_effectPhase > 5)
    {
        m_effectPhase = 0;
    }
}

// -----------------------------------------------------------------------------
// Sparkle
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateSparkle() noexcept
{
    /*
     * =========================================================
     * SPARKLE
     * =========================================================
     *
     * Efecto de destellos aleatorios.
     *
     * Cada LED puede encenderse con una intensidad diferente,
     * creando una apariencia de pequeñas chispas.
     *
     * El efecto utiliza LedCount, por lo que no depende
     * directamente de que existan 6 LEDs.
     */

    static constexpr std::uint32_t SparkleStepMs = 80;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        SparkleStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        /*
         * La probabilidad determina qué tan frecuente
         * aparece un destello en cada LED.
         */
        const std::uint8_t sparkle =
            random(0, 256);

        if (sparkle < 70)
        {
            m_strip.setPixelColor(
                led,
                0,
                0,
                0);
            continue;
        }

        /*
         * Cada destello tiene una intensidad aleatoria.
         */
        const std::uint8_t brightness =
            random(80, 256);

        const std::uint16_t scaledRed =
            (static_cast<std::uint16_t>(red) *
             brightness) / 255;

        const std::uint16_t scaledGreen =
            (static_cast<std::uint16_t>(green) *
             brightness) / 255;

        const std::uint16_t scaledBlue =
            (static_cast<std::uint16_t>(blue) *
             brightness) / 255;

        m_strip.setPixelColor(
            led,
            scaledRed,
            scaledGreen,
            scaledBlue);
    }

    m_strip.show();
}

// -----------------------------------------------------------------------------
// LIGHTNING
// -----------------------------------------------------------------------------


void AmbientLightController::UpdateLightning() noexcept
{
    /*
     * =========================================================
     * LIGHTNING
     * =========================================================
     *
     * Simula un relámpago:
     *
     *      ⚡ DESTELLO
     *      ·······
     *      ⚡ DESTELLO CORTO
     *      ···············
     *
     * La secuencia es irregular para evitar que parezca
     * simplemente un FastBlink.
     *
     * Todos los LEDs participan simultáneamente.
     */

    static constexpr std::uint32_t LightningStepMs = 70;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        LightningStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    /*
     * =========================================================
     * SECUENCIA
     * =========================================================
     *
     * Fase 0:
     *      Destello fuerte
     *
     * Fase 1:
     *      Apagado
     *
     * Fase 2:
     *      Segundo destello
     *
     * Fase 3-5:
     *      Pausa
     */

    std::uint8_t brightness = 0;

    switch (m_effectPhase)
    {
    case 0:
        brightness = 255;
        break;

    case 1:
        brightness = 0;
        break;

    case 2:
        brightness = 180;
        break;

    case 3:
        brightness = 0;
        break;

    case 4:
        brightness = 0;
        break;

    case 5:
        brightness = 0;
        break;

    default:
        m_effectPhase = 0;
        brightness = 0;
        break;
    }

    const std::uint16_t scaledRed =
        (static_cast<std::uint16_t>(red) *
         brightness) / 255;

    const std::uint16_t scaledGreen =
        (static_cast<std::uint16_t>(green) *
         brightness) / 255;

    const std::uint16_t scaledBlue =
        (static_cast<std::uint16_t>(blue) *
         brightness) / 255;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        m_strip.setPixelColor(
            led,
            scaledRed,
            scaledGreen,
            scaledBlue);
    }

    m_strip.show();

    ++m_effectPhase;

    if (m_effectPhase > 5)
    {
        m_effectPhase = 0;
    }
}

// -----------------------------------------------------------------------------
// Fire
// -----------------------------------------------------------------------------


void AmbientLightController::UpdateFire() noexcept
{
    /*
     * =========================================================
     * FIRE
     * =========================================================
     *
     * Simula una llama mediante variaciones independientes
     * de intensidad en cada LED.
     *
     * Cada actualización genera una nueva intensidad para
     * cada LED, creando un movimiento orgánico y aleatorio.
     *
     * El efecto utiliza LedCount, por lo que no depende
     * directamente de que existan 6 LEDs.
     */

    static constexpr std::uint32_t FireStepMs = 80;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        FireStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        /*
         * La intensidad base evita que las llamas se apaguen
         * completamente.
         */
        const std::uint8_t brightness =
            random(70, 256);

        const std::uint16_t scaledRed =
            (static_cast<std::uint16_t>(red) *
             brightness) / 255;

        const std::uint16_t scaledGreen =
            (static_cast<std::uint16_t>(green) *
             brightness) / 255;

        const std::uint16_t scaledBlue =
            (static_cast<std::uint16_t>(blue) *
             brightness) / 255;

        m_strip.setPixelColor(
            led,
            scaledRed,
            scaledGreen,
            scaledBlue);
    }

    m_strip.show();
}



// -----------------------------------------------------------------------------
// Rainbow chase
// -----------------------------------------------------------------------------


void AmbientLightController::UpdateRainbowChase() noexcept
{
    /*
     * =========================================================
     * RAINBOW CHASE
     * =========================================================
     *
     * El arcoíris se desplaza a través de los LEDs:
     *
     *      🌈
     *
     *      🔴 🟠 🟡 🟢 🔵 🟣
     *        ↓
     *      🟣 🔴 🟠 🟡 🟢 🔵
     *        ↓
     *      🔵 🟣 🔴 🟠 🟡 🟢
     *
     * Cada LED representa una sección diferente del arcoíris.
     *
     * El efecto utiliza LedCount, por lo que no depende
     * directamente de que existan 6 LEDs.
     */

    static constexpr std::uint32_t RainbowChaseStepMs = 5;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        RainbowChaseStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    /*
     * La rueda de color se divide entre los LEDs.
     */
    static constexpr std::uint8_t ColorSpacing =
        256 / LedCount;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const std::uint8_t colorIndex =
            static_cast<std::uint8_t>(
                (m_effectPhase +
                 (led * ColorSpacing)) &
                0xFF);

        /*
         * Conversión HSV simplificada:
         * Hue -> RGB
         */
        const std::uint8_t region =
            colorIndex / 43;

        const std::uint8_t remainder =
            (colorIndex - (region * 43)) * 6;

        const std::uint8_t p = 0;
        const std::uint8_t q =
            255 - remainder;
        const std::uint8_t t =
            remainder;

        std::uint8_t red = 0;
        std::uint8_t green = 0;
        std::uint8_t blue = 0;

        switch (region)
        {
        case 0:
            red = 255;
            green = t;
            blue = p;
            break;

        case 1:
            red = q;
            green = 255;
            blue = p;
            break;

        case 2:
            red = p;
            green = 255;
            blue = t;
            break;

        case 3:
            red = p;
            green = q;
            blue = 255;
            break;

        case 4:
            red = t;
            green = p;
            blue = 255;
            break;

        default:
            red = 255;
            green = p;
            blue = q;
            break;
        }

        m_strip.setPixelColor(
            led,
            red,
            green,
            blue);
    }

    m_strip.show();

    /*
     * Desplazamos el arcoíris.
     */
    ++m_effectPhase;
}

// -----------------------------------------------------------------------------
// Rainbow wave
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateRainbowWave() noexcept
{
    /*
     * =========================================================
     * RAINBOW WAVE
     * =========================================================
     *
     * Genera una onda continua de colores a través de los LEDs.
     *
     * A diferencia de RainbowChase:
     *
     *      RainbowChase:
     *      🔴 🟠 🟡 🟢 🔵 🟣
     *      → desplazamiento →
     *
     *      RainbowWave:
     *      🔴 🟠 🟡 🟢 🔵 🟣
     *       🟠 🟡 🟢 🔵 🟣 🔴
     *        🟡 🟢 🔵 🟣 🔴 🟠
     *
     * El cambio es continuo y produce una sensación de onda.
     */

    static constexpr std::uint32_t RainbowWaveStepMs = 40;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        RainbowWaveStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    /*
     * Separación entre los colores de cada LED.
     *
     * Con 6 LEDs:
     *
     *      0 → 42 → 84 → 126 → 168 → 210
     */
    static constexpr std::uint8_t ColorSpacing =
        256 / LedCount;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const std::uint8_t colorIndex =
            static_cast<std::uint8_t>(
                (m_effectPhase +
                 (led * ColorSpacing)) &
                0xFF);

        const std::uint8_t region =
            colorIndex / 43;

        const std::uint8_t remainder =
            (colorIndex -
             (region * 43)) * 6;

        const std::uint8_t p = 0;

        const std::uint8_t q =
            255 - remainder;

        const std::uint8_t t =
            remainder;

        std::uint8_t red = 0;
        std::uint8_t green = 0;
        std::uint8_t blue = 0;

        switch (region)
        {
        case 0:
            red = 255;
            green = t;
            blue = p;
            break;

        case 1:
            red = q;
            green = 255;
            blue = p;
            break;

        case 2:
            red = p;
            green = 255;
            blue = t;
            break;

        case 3:
            red = p;
            green = q;
            blue = 255;
            break;

        case 4:
            red = t;
            green = p;
            blue = 255;
            break;

        default:
            red = 255;
            green = p;
            blue = q;
            break;
        }

        m_strip.setPixelColor(
            led,
            red,
            green,
            blue);
    }

    m_strip.show();

    /*
     * Avanzamos lentamente la onda.
     */
    ++m_effectPhase;
}

// -----------------------------------------------------------------------------
// Center out
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateCenterOut() noexcept
{
    /*
     * =========================================================
     * CENTER OUT
     * =========================================================
     *
     * La luz nace en el centro de la tira y se expande hacia
     * los extremos.
     *
     * Con 6 LEDs:
     *
     *      1  2  3  4  5  6
     *      ·  ·  █  █  ·  ·
     *      ·  █  █  █  █  ·
     *      █  █  █  █  █  █
     *
     * Después comienza nuevamente desde el centro.
     *
     * El efecto utiliza LedCount para mantenerlo adaptable
     * a diferentes cantidades de LEDs.
     */

    static constexpr std::uint32_t CenterOutStepMs = 100;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        CenterOutStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    /*
     * =========================================================
     * CALCULAR DISTANCIA AL CENTRO
     * =========================================================
     *
     * Para 6 LEDs:
     *
     *      LED:       0  1  2  3  4  5
     *      Distancia: 3  2  1  0  1  2
     *
     * La fase determina hasta qué distancia desde el centro
     * permanece encendido cada LED.
     */

    const std::uint8_t center =
        LedCount / 2;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const std::uint8_t distance =
            (led < center)
                ? (center - led)
                : (led - center);

        if (distance <= m_effectPhase)
        {
            m_strip.setPixelColor(
                led,
                red,
                green,
                blue);
        }
        else
        {
            m_strip.setPixelColor(
                led,
                0,
                0,
                0);
        }
    }

    m_strip.show();

    ++m_effectPhase;

    /*
     * Cuando llegamos al extremo, comenzamos nuevamente
     * desde el centro.
     */
    if (m_effectPhase > center)
    {
        m_effectPhase = 0;
    }
}

// -----------------------------------------------------------------------------
// Explosion
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateExplosion() noexcept
{
    /*
     * =========================================================
     * EXPLOSION
     * =========================================================
     *
     * Simula una explosión de luz:
     *
     *          💥
     *      ·  █  █  ·
     *      █  █  █  █
     *      █  █  █  █
     *          ↓
     *      ·  ·  ·  ·
     *
     * La luz nace en el centro, se expande rápidamente y
     * después desaparece.
     *
     * El efecto utiliza LedCount para adaptarse a diferentes
     * cantidades de LEDs.
     */

    static constexpr std::uint32_t ExplosionStepMs = 80;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        ExplosionStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    /*
     * =========================================================
     * FASE DE LA EXPLOSIÓN
     * =========================================================
     *
     *      0 = centro
     *      1 = expansión
     *      2 = expansión máxima
     *      3 = destello máximo
     *      4 = caída
     *      5 = apagado
     */

    std::uint8_t brightness = 0;

    switch (m_effectPhase)
    {
    case 0:
        brightness = 255;
        break;

    case 1:
        brightness = 255;
        break;

    case 2:
        brightness = 220;
        break;

    case 3:
        brightness = 255;
        break;

    case 4:
        brightness = 100;
        break;

    case 5:
        brightness = 0;
        break;

    default:
        m_effectPhase = 0;
        brightness = 255;
        break;
    }

    /*
     * =========================================================
     * EXPANSIÓN
     * =========================================================
     *
     * La cantidad de LEDs encendidos aumenta conforme avanza
     * la fase.
     */

    const std::uint8_t center =
        LedCount / 2;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const std::uint8_t distance =
            (led < center)
                ? (center - led)
                : (led - center);

        bool visible = false;

        switch (m_effectPhase)
        {
        case 0:
            visible = distance <= 0;
            break;

        case 1:
            visible = distance <= 1;
            break;

        case 2:
            visible = distance <= 2;
            break;

        case 3:
            visible = true;
            break;

        case 4:
            visible = true;
            break;

        case 5:
            visible = false;
            break;

        case 6:
            visible = false;
            break;

        case 7:
            visible = false;
            break;

        default:
            visible = false;
            break;
        }

        if (!visible)
        {
            m_strip.setPixelColor(
                led,
                0,
                0,
                0);

            continue;
        }

        const std::uint16_t scaledRed =
            (static_cast<std::uint16_t>(red) *
             brightness) / 255;

        const std::uint16_t scaledGreen =
            (static_cast<std::uint16_t>(green) *
             brightness) / 255;

        const std::uint16_t scaledBlue =
            (static_cast<std::uint16_t>(blue) *
             brightness) / 255;

        m_strip.setPixelColor(
            led,
            scaledRed,
            scaledGreen,
            scaledBlue);
    }

    m_strip.show();

    ++m_effectPhase;

    if (m_effectPhase > 5)
    {
        m_effectPhase = 0;
    }
}

// -----------------------------------------------------------------------------
// Rainbow Explosion
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateRainbowExplosion() noexcept
{
    /*
     * =========================================================
     * RAINBOW EXPLOSION
     * =========================================================
     *
     * Una explosión de luz que nace en el centro y se expande
     * hacia los extremos mientras el color cambia.
     *
     *      · · █ █ · ·
     *      · █ █ █ █ ·
     *      █ █ █ █ █ █
     *
     * El color avanza continuamente por la rueda RGB.
     *
     * La fase controla la expansión y m_effectBrightness
     * no es necesario para este efecto.
     */

    static constexpr std::uint32_t
        RainbowExplosionStepMs = 50;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        RainbowExplosionStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    /*
     * =========================================================
     * COLOR ARCOÍRIS
     * =========================================================
     */

    const std::uint8_t hue =
        static_cast<std::uint8_t>(
            m_effectPhase * 42);

    const std::uint8_t region =
        hue / 43;

    const std::uint8_t remainder =
        (hue - (region * 43)) * 6;

    const std::uint8_t q =
        255 - remainder;

    const std::uint8_t t =
        remainder;

    std::uint8_t red = 0;
    std::uint8_t green = 0;
    std::uint8_t blue = 0;

    switch (region)
    {
    case 0:
        red = 255;
        green = t;
        blue = 0;
        break;

    case 1:
        red = q;
        green = 255;
        blue = 0;
        break;

    case 2:
        red = 0;
        green = 255;
        blue = t;
        break;

    case 3:
        red = 0;
        green = q;
        blue = 255;
        break;

    case 4:
        red = t;
        green = 0;
        blue = 255;
        break;

    default:
        red = 255;
        green = 0;
        blue = q;
        break;
    }

    /*
     * =========================================================
     * FASE DE EXPANSIÓN
     * =========================================================
     *
     *      0 = centro
     *      1 = expansión
     *      2 = expansión
     *      3 = máxima expansión
     *      4 = destello
     *      5 = apagado
     */

    std::uint8_t brightness = 255;

    switch (m_effectPhase)
    {
    case 0:
        brightness = 255;
        break;

    case 1:
        brightness = 255;
        break;

    case 2:
        brightness = 220;
        break;

    case 3:
        brightness = 255;
        break;

    case 4:
        brightness = 120;
        break;

    case 5:
        brightness = 0;
        break;

    default:
        m_effectPhase = 0;
        brightness = 255;
        break;
    }

    const std::uint8_t center =
        LedCount / 2;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const std::uint8_t distance =
            (led < center)
                ? (center - led)
                : (led - center);

        bool visible = false;

        switch (m_effectPhase)
        {
        case 0:
            visible = distance <= 0;
            break;

        case 1:
            visible = distance <= 1;
            break;

        case 2:
            visible = distance <= 2;
            break;

        case 3:
        case 4:
            visible = true;
            break;

        case 5:
            visible = false;
            break;

        default:
            visible = false;
            break;
        }

        if (!visible)
        {
            m_strip.setPixelColor(
                led,
                0,
                0,
                0);

            continue;
        }

        const std::uint16_t scaledRed =
            (static_cast<std::uint16_t>(red) *
             brightness) / 255;

        const std::uint16_t scaledGreen =
            (static_cast<std::uint16_t>(green) *
             brightness) / 255;

        const std::uint16_t scaledBlue =
            (static_cast<std::uint16_t>(blue) *
             brightness) / 255;

        m_strip.setPixelColor(
            led,
            scaledRed,
            scaledGreen,
            scaledBlue);
    }

    m_strip.show();

    /*
     * Avanzamos la fase y, por lo tanto, el color.
     */
    ++m_effectPhase;

    if (m_effectPhase > 5)
    {
        m_effectPhase = 0;
    }
}

// -----------------------------------------------------------------------------
// Center pulse
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateCenterPulse() noexcept
{
    /*
     * =========================================================
     * CENTER PULSE
     * =========================================================
     *
     * Un pulso nace en el centro y se expande hacia los
     * extremos, para después desaparecer.
     *
     *      · · █ █ · ·
     *      · █ █ █ █ ·
     *      █ █ █ █ █ █
     *      · █ █ █ █ ·
     *      · · █ █ · ·
     *      · · · · · ·
     *
     * A diferencia de CenterOut, aquí la expansión es
     * temporal y vuelve a comenzar desde el centro.
     */

    static constexpr std::uint32_t CenterPulseStepMs = 70;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        CenterPulseStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    const std::uint8_t center =
        LedCount / 2;

    /*
     * La fase representa el radio actual del pulso.
     *
     * Para 6 LEDs:
     *
     *      0 → centro
     *      1 → expansión
     *      2 → expansión máxima
     *      3 → desaparición
     */

    std::uint8_t radius = 0;
    std::uint8_t brightness = 255;

    switch (m_effectPhase)
    {
    case 0:
        radius = 0;
        brightness = 255;
        break;

    case 1:
        radius = 1;
        brightness = 230;
        break;

    case 2:
        radius = 2;
        brightness = 180;
        break;

    case 3:
        radius = 3;
        brightness = 100;
        break;

    case 4:
        radius = 4;
        brightness = 0;
        break;

    default:
        m_effectPhase = 0;
        radius = 0;
        brightness = 255;
        break;
    }

    const std::uint16_t scaledRed =
        (static_cast<std::uint16_t>(red) *
         brightness) / 255;

    const std::uint16_t scaledGreen =
        (static_cast<std::uint16_t>(green) *
         brightness) / 255;

    const std::uint16_t scaledBlue =
        (static_cast<std::uint16_t>(blue) *
         brightness) / 255;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const std::uint8_t distance =
            (led < center)
                ? (center - led)
                : (led - center);

        if (distance <= radius)
        {
            m_strip.setPixelColor(
                led,
                scaledRed,
                scaledGreen,
                scaledBlue);
        }
        else
        {
            m_strip.setPixelColor(
                led,
                0,
                0,
                0);
        }
    }

    m_strip.show();

    ++m_effectPhase;

    if (m_effectPhase > 4)
    {
        m_effectPhase = 0;
    }
}

// -----------------------------------------------------------------------------
// Ripple
// -----------------------------------------------------------------------------


void AmbientLightController::UpdateRipple() noexcept
{
    /*
     * =========================================================
     * RIPPLE
     * =========================================================
     *
     * Una onda de luz recorre los LEDs:
     *
     *      █ · · · · ·
     *      █ █ · · · ·
     *      · █ █ · · ·
     *      · · █ █ · ·
     *      · · · █ █ ·
     *      · · · · █ █
     *      · · · · · █
     *
     * La onda tiene una cabeza brillante y una pequeña cola
     * que se desvanece detrás de ella.
     */

    static constexpr std::uint32_t RippleStepMs = 70;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        RippleStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    /*
     * =========================================================
     * LONGITUD DE LA ONDA
     * =========================================================
     */

    static constexpr std::uint8_t RippleLength = 3;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        /*
         * Distancia desde la posición actual de la onda.
         */
        const std::int16_t distance =
            static_cast<std::int16_t>(led) -
            static_cast<std::int16_t>(m_effectPhase);

        std::uint8_t brightness = 0;

        /*
         * Cabeza de la onda.
         */
        if (distance == 0)
        {
            brightness = 255;
        }
        /*
         * Primera parte de la cola.
         */
        else if (distance == -1)
        {
            brightness = 150;
        }
        /*
         * Segunda parte de la cola.
         */
        else if (distance == -2)
        {
            brightness = 60;
        }

        if (brightness == 0)
        {
            m_strip.setPixelColor(
                led,
                0,
                0,
                0);

            continue;
        }

        const std::uint16_t scaledRed =
            (static_cast<std::uint16_t>(red) *
             brightness) / 255;

        const std::uint16_t scaledGreen =
            (static_cast<std::uint16_t>(green) *
             brightness) / 255;

        const std::uint16_t scaledBlue =
            (static_cast<std::uint16_t>(blue) *
             brightness) / 255;

        m_strip.setPixelColor(
            led,
            scaledRed,
            scaledGreen,
            scaledBlue);
    }

    m_strip.show();

    ++m_effectPhase;

    /*
     * Dejamos que la onda salga completamente de la tira
     * antes de comenzar nuevamente.
     */
    if (m_effectPhase >=
        (LedCount + RippleLength))
    {
        m_effectPhase = 0;
    }
}


// -----------------------------------------------------------------------------
// Alternating wave
// -----------------------------------------------------------------------------


void AmbientLightController::UpdateAlternatingWave() noexcept
{
    /*
     * =========================================================
     * ALTERNATING WAVE
     * =========================================================
     *
     * Dos puntos de luz avanzan simultáneamente desde los
     * extremos hacia el centro y después regresan.
     *
     *      1  2  3  4  5  6
     *
     *      █  ·  ·  ·  ·  █
     *      ·  █  ·  ·  █  ·
     *      ·  ·  █  █  ·  ·
     *      ·  █  ·  ·  █  ·
     *      █  ·  ·  ·  ·  █
     *
     * El movimiento utiliza la secuencia física de la tira.
     */

    static constexpr std::uint32_t
        AlternatingWaveStepMs = 100;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        AlternatingWaveStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    /*
     * Distancia actual desde los extremos.
     *
     * Para 6 LEDs:
     *
     *      0 → 1 → 2 → 1 → 0
     *
     * Esto produce:
     *
     *      1,6
     *      2,5
     *      3,4
     *      2,5
     *      1,6
     */

    const std::uint8_t wavePosition =
        m_effectPhase;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const std::uint8_t distanceFromStart =
            led;

        const std::uint8_t distanceFromEnd =
            (LedCount - 1) - led;

        const std::uint8_t distance =
            (distanceFromStart <
             distanceFromEnd)
                ? distanceFromStart
                : distanceFromEnd;

        if (distance == wavePosition)
        {
            m_strip.setPixelColor(
                led,
                red,
                green,
                blue);
        }
        else
        {
            m_strip.setPixelColor(
                led,
                0,
                0,
                0);
        }
    }

    m_strip.show();

    ++m_effectPhase;

    /*
     * Para una tira de 6 LEDs:
     *
     *      0 → 1 → 2 → 3 → 4
     *
     * Las posiciones 2 y 3 representan el centro.
     * Reiniciamos para volver a los extremos.
     */
    if (m_effectPhase >=
        ((LedCount + 1) / 2))
    {
        m_effectPhase = 0;
    }
}


// -----------------------------------------------------------------------------
// Strobe
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateStrobe() noexcept
{
    /*
     * =========================================================
     * STROBE
     * =========================================================
     *
     * Efecto de estrobo rápido.
     *
     *      ██████
     *      ······
     *      ██████
     *      ······
     *
     * A diferencia de FastBlink, el cambio es mucho más
     * agresivo y utiliza dos niveles de intensidad.
     *
     * Todos los LEDs trabajan simultáneamente.
     */

    static constexpr std::uint32_t StrobeStepMs = 45;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        StrobeStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    /*
     * Alternamos entre un destello máximo y uno ligeramente
     * más tenue.
     */
    const std::uint8_t brightness =
        m_effectState
            ? 255
            : 180;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const std::uint16_t scaledRed =
            (static_cast<std::uint16_t>(red) *
             brightness) / 255;

        const std::uint16_t scaledGreen =
            (static_cast<std::uint16_t>(green) *
             brightness) / 255;

        const std::uint16_t scaledBlue =
            (static_cast<std::uint16_t>(blue) *
             brightness) / 255;

        m_strip.setPixelColor(
            led,
            scaledRed,
            scaledGreen,
            scaledBlue);
    }

    m_strip.show();

    m_effectState = !m_effectState;
}

// -----------------------------------------------------------------------------
// DoubleFlash
// -----------------------------------------------------------------------------


void AmbientLightController::UpdateDoubleFlash() noexcept
{
    /*
     * =========================================================
     * DOUBLE FLASH
     * =========================================================
     *
     * Patrón:
     *
     *      ██████
     *      ······
     *      ██████
     *      ···········
     *
     * Dos destellos rápidos seguidos de una pausa.
     */

    static constexpr std::uint32_t DoubleFlashStepMs = 90;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        DoubleFlashStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    std::uint8_t brightness = 0;

    switch (m_effectPhase)
    {
    case 0:
        brightness = 255;
        break;

    case 1:
        brightness = 0;
        break;

    case 2:
        brightness = 255;
        break;

    case 3:
        brightness = 0;
        break;

    case 4:
        brightness = 0;
        break;

    case 5:
        brightness = 0;
        break;

    default:
        m_effectPhase = 0;
        brightness = 255;
        break;
    }

    const std::uint16_t scaledRed =
        (static_cast<std::uint16_t>(red) *
         brightness) / 255;

    const std::uint16_t scaledGreen =
        (static_cast<std::uint16_t>(green) *
         brightness) / 255;

    const std::uint16_t scaledBlue =
        (static_cast<std::uint16_t>(blue) *
         brightness) / 255;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        m_strip.setPixelColor(
            led,
            scaledRed,
            scaledGreen,
            scaledBlue);
    }

    m_strip.show();

    ++m_effectPhase;

    if (m_effectPhase > 5)
    {
        m_effectPhase = 0;
    }
}


// -----------------------------------------------------------------------------
// Meteor
// -----------------------------------------------------------------------------


void AmbientLightController::UpdateMeteor() noexcept
{
    /*
     * =========================================================
     * METEOR
     * =========================================================
     *
     * Un punto brillante recorre la cadena de LEDs
     * dejando una cola progresivamente más tenue.
     *
     *      ● · · · · ·
     *      ●● · · · ·
     *      ·●● · · · ·
     *      ··●● · · ·
     *      ···●● · ·
     *
     * La cabeza del meteorito siempre es la parte
     * más brillante.
     */

    static constexpr std::uint32_t MeteorStepMs = 80;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        MeteorStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const std::int16_t distance =
            static_cast<std::int16_t>(m_effectPhase) -
            static_cast<std::int16_t>(led);

        std::uint8_t brightness = 0;

        switch (distance)
        {
        case 0:
            brightness = 255;
            break;

        case 1:
            brightness = 150;
            break;

        case 2:
            brightness = 60;
            break;

        default:
            brightness = 0;
            break;
        }

        const std::uint16_t scaledRed =
            (static_cast<std::uint16_t>(red) *
             brightness) / 255;

        const std::uint16_t scaledGreen =
            (static_cast<std::uint16_t>(green) *
             brightness) / 255;

        const std::uint16_t scaledBlue =
            (static_cast<std::uint16_t>(blue) *
             brightness) / 255;

        m_strip.setPixelColor(
            led,
            scaledRed,
            scaledGreen,
            scaledBlue);
    }

    m_strip.show();

    ++m_effectPhase;

    if (m_effectPhase >=
        (LedCount + 3))
    {
        m_effectPhase = 0;
    }
}

// -----------------------------------------------------------------------------
// Breathing
// -----------------------------------------------------------------------------


void AmbientLightController::UpdateBreathing() noexcept
{
    /*
     * =========================================================
     * BREATHING
     * =========================================================
     *
     * Respiración suave de todos los LEDs.
     *
     *      ······
     *      ░░░░░░
     *      ▒▒▒▒▒▒
     *      ██████
     *      ▒▒▒▒▒▒
     *      ░░░░░░
     *      ······
     *
     * La intensidad aumenta y disminuye progresivamente.
     */

    static constexpr std::uint32_t BreathingStepMs = 15;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        BreathingStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    if (m_effectDirection > 0)
    {
        if (m_effectBrightness < 255)
        {
            m_effectBrightness += 5;
        }
        else
        {
            m_effectDirection = -1;
        }
    }
    else
    {
        if (m_effectBrightness > 0)
        {
            m_effectBrightness -= 5;
        }
        else
        {
            m_effectDirection = 1;
        }
    }

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    const std::uint8_t brightness =
        static_cast<std::uint8_t>(
            m_effectBrightness);

    const std::uint16_t scaledRed =
        (static_cast<std::uint16_t>(red) *
         brightness) / 255;

    const std::uint16_t scaledGreen =
        (static_cast<std::uint16_t>(green) *
         brightness) / 255;

    const std::uint16_t scaledBlue =
        (static_cast<std::uint16_t>(blue) *
         brightness) / 255;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        m_strip.setPixelColor(
            led,
            scaledRed,
            scaledGreen,
            scaledBlue);
    }

    m_strip.show();
}

// -----------------------------------------------------------------------------
// Comet
// -----------------------------------------------------------------------------


void AmbientLightController::UpdateComet() noexcept
{
    /*
     * =========================================================
     * COMET
     * =========================================================
     *
     * Una cabeza brillante recorre los LEDs y deja
     * una estela progresivamente más tenue.
     *
     *      ● · · · · ·
     *      ●● · · · ·
     *      ●●● · · ·
     *      ·●●● · ·
     *      ··●●● ·
     *      ···●●●
     *
     * Al llegar al extremo, invierte la dirección.
     */

    static constexpr std::uint32_t CometStepMs = 70;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        CometStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const std::int16_t distance =
            static_cast<std::int16_t>(led) -
            static_cast<std::int16_t>(m_effectPhase);

        std::uint8_t brightness = 0;

        switch (distance)
        {
        case 0:
            brightness = 255;
            break;

        case -1:
        case 1:
            brightness = 140;
            break;

        case -2:
        case 2:
            brightness = 55;
            break;

        default:
            brightness = 0;
            break;
        }

        const std::uint16_t scaledRed =
            (static_cast<std::uint16_t>(red) *
             brightness) / 255;

        const std::uint16_t scaledGreen =
            (static_cast<std::uint16_t>(green) *
             brightness) / 255;

        const std::uint16_t scaledBlue =
            (static_cast<std::uint16_t>(blue) *
             brightness) / 255;

        m_strip.setPixelColor(
            led,
            scaledRed,
            scaledGreen,
            scaledBlue);
    }

    m_strip.show();

    if (m_effectDirection > 0)
    {
        if (m_effectPhase >= (LedCount - 1))
        {
            m_effectDirection = -1;
        }
        else
        {
            ++m_effectPhase;
        }
    }
    else
    {
        if (m_effectPhase == 0)
        {
            m_effectDirection = 1;
        }
        else
        {
            --m_effectPhase;
        }
    }
}

// -----------------------------------------------------------------------------
// Wave
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateWave() noexcept
{
    /*
     * =========================================================
     * WAVE
     * =========================================================
     *
     * Una onda de intensidad recorre todos los LEDs.
     *
     *      ████░░
     *      ░████░
     *      ░░████
     *      ░████░
     *
     * Varios LEDs permanecen encendidos al mismo tiempo,
     * creando una transición suave.
     */

    static constexpr std::uint32_t WaveStepMs = 50;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        WaveStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const std::uint8_t position =
            static_cast<std::uint8_t>(
                (led * 32 + m_effectPhase) & 0xFF);

        std::uint8_t brightness = 0;

        if (position < 64)
        {
            brightness = position * 4;
        }
        else if (position < 128)
        {
            brightness = 255;
        }
        else if (position < 192)
        {
            brightness =
                255 - ((position - 128) * 4);
        }
        else
        {
            brightness = 0;
        }

        const std::uint16_t scaledRed =
            (static_cast<std::uint16_t>(red) *
             brightness) / 255;

        const std::uint16_t scaledGreen =
            (static_cast<std::uint16_t>(green) *
             brightness) / 255;

        const std::uint16_t scaledBlue =
            (static_cast<std::uint16_t>(blue) *
             brightness) / 255;

        m_strip.setPixelColor(
            led,
            scaledRed,
            scaledGreen,
            scaledBlue);
    }

    m_strip.show();

    m_effectPhase += 5;
}


// -----------------------------------------------------------------------------
// Theater
// -----------------------------------------------------------------------------


void AmbientLightController::UpdateTheater() noexcept
{
    /*
     * =========================================================
     * THEATER
     * =========================================================
     *
     * Los LEDs se encienden desde los extremos hacia
     * el centro y después regresan hacia afuera.
     *
     *      █····█
     *      ██··██
     *      ██████
     *      ██··██
     *      █····█
     *
     * Produce una sensación de apertura y cierre.
     */

    static constexpr std::uint32_t TheaterStepMs = 100;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        TheaterStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    const std::uint8_t center =
        LedCount / 2;

    for (std::uint8_t led = 0;
         led < LedCount;
         ++led)
    {
        const std::uint8_t distanceFromEdge =
            (led < center)
                ? led
                : (LedCount - 1 - led);

        const bool visible =
            distanceFromEdge <= m_effectPhase;

        if (visible)
        {
            m_strip.setPixelColor(
                led,
                red,
                green,
                blue);
        }
        else
        {
            m_strip.setPixelColor(
                led,
                0,
                0,
                0);
        }
    }

    m_strip.show();

    if (m_effectDirection > 0)
    {
        ++m_effectPhase;

        if (m_effectPhase >= center)
        {
            m_effectDirection = -1;
        }
    }
    else
    {
        if (m_effectPhase == 0)
        {
            m_effectDirection = 1;
        }
        else
        {
            --m_effectPhase;
        }
    }
}

// -----------------------------------------------------------------------------
// Side fade
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateSideFade() noexcept
{
    /*
     * =========================================================
     * SIDE FADE
     * =========================================================
     *
     * Realiza un fundido entre los dos lados del Kart.
     *
     *      IZQUIERDA        DERECHA
     *
     *      ███              ···
     *      ███              ···
     *      ███              ···
     *
     *              ↓
     *
     *      ▒▒▒              ▒▒▒
     *      ▒▒▒              ▒▒▒
     *      ▒▒▒              ▒▒▒
     *
     *              ↓
     *
     *      ···              ███
     *      ···              ███
     *      ···              ███
     *
     * Después invierte el proceso.
     */

    static constexpr std::uint32_t SideFadeStepMs = 20;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        SideFadeStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    if (m_effectDirection > 0)
    {
        if (m_effectBrightness < 255)
        {
            m_effectBrightness += 5;
        }
        else
        {
            m_effectDirection = -1;
        }
    }
    else
    {
        if (m_effectBrightness > 0)
        {
            m_effectBrightness -= 5;
        }
        else
        {
            m_effectDirection = 1;
        }
    }

    const std::uint8_t rightBrightness =
        static_cast<std::uint8_t>(
            m_effectBrightness);

    const std::uint8_t leftBrightness =
        static_cast<std::uint8_t>(
            255 - m_effectBrightness);

    const std::uint16_t leftRed =
        (static_cast<std::uint16_t>(red) *
         leftBrightness) / 255;

    const std::uint16_t leftGreen =
        (static_cast<std::uint16_t>(green) *
         leftBrightness) / 255;

    const std::uint16_t leftBlue =
        (static_cast<std::uint16_t>(blue) *
         leftBrightness) / 255;

    const std::uint16_t rightRed =
        (static_cast<std::uint16_t>(red) *
         rightBrightness) / 255;

    const std::uint16_t rightGreen =
        (static_cast<std::uint16_t>(green) *
         rightBrightness) / 255;

    const std::uint16_t rightBlue =
        (static_cast<std::uint16_t>(blue) *
         rightBrightness) / 255;

    /*
     * ---------------------------------------------------------
     * LADO IZQUIERDO
     * LEDs 1, 2 y 3
     * ---------------------------------------------------------
     */

    for (std::uint8_t led = 0;
         led < 3;
         ++led)
    {
        m_strip.setPixelColor(
            led,
            leftRed,
            leftGreen,
            leftBlue);
    }

    /*
     * ---------------------------------------------------------
     * LADO DERECHO
     * LEDs 4, 5 y 6
     * ---------------------------------------------------------
     */

    for (std::uint8_t led = 3;
         led < LedCount;
         ++led)
    {
        m_strip.setPixelColor(
            led,
            rightRed,
            rightGreen,
            rightBlue);
    }

    m_strip.show();
}

// -----------------------------------------------------------------------------
// Mirror Wave
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateMirrorWave() noexcept
{
    /*
     * =========================================================
     * MIRROR WAVE
     * =========================================================
     *
     * Onda simétrica basada en la geometría física del Kart.
     *
     *      (1,6)
     *        ↓
     *      (2,5)
     *        ↓
     *      (3,4)
     *        ↓
     *      (2,5)
     *        ↓
     *      (1,6)
     *
     * Cada pareja de LEDs se ilumina simultáneamente.
     */

    static constexpr std::uint32_t MirrorWaveStepMs = 100;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        MirrorWaveStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    const std::uint8_t pairCount =
        LedCount / 2;

    /*
     * ---------------------------------------------------------
     * Determinar qué pareja está activa.
     * ---------------------------------------------------------
     */

    const std::uint8_t pair =
        m_effectPhase;

    /*
     * ---------------------------------------------------------
     * Apagar todos los LEDs.
     * ---------------------------------------------------------
     */

    m_strip.clear();

    /*
     * ---------------------------------------------------------
     * Pareja simétrica:
     *
     * pair 0 → LEDs 1 y 6
     * pair 1 → LEDs 2 y 5
     * pair 2 → LEDs 3 y 4
     * ---------------------------------------------------------
     */

    const std::uint8_t leftLed =
        pair;

    const std::uint8_t rightLed =
        (LedCount - 1) - pair;

    m_strip.setPixelColor(
        leftLed,
        red,
        green,
        blue);

    m_strip.setPixelColor(
        rightLed,
        red,
        green,
        blue);

    m_strip.show();

    /*
     * ---------------------------------------------------------
     * Avanzar hacia el centro y después regresar.
     * ---------------------------------------------------------
     */

    if (m_effectDirection > 0)
    {
        if (m_effectPhase >=
            (pairCount - 1))
        {
            m_effectDirection = -1;
        }
        else
        {
            ++m_effectPhase;
        }
    }
    else
    {
        if (m_effectPhase == 0)
        {
            m_effectDirection = 1;
        }
        else
        {
            --m_effectPhase;
        }
    }
}

// -----------------------------------------------------------------------------
// Center Split
// -----------------------------------------------------------------------------


void AmbientLightController::UpdateCenterSplit() noexcept
{
    /*
     * =========================================================
     * CENTER SPLIT
     * =========================================================
     *
     *      (3,4)
     *        ↓
     *      (2,5)
     *        ↓
     *      (1,6)
     *        ↓
     *      (2,5)
     *        ↓
     *      (3,4)
     */

    static constexpr std::uint32_t CenterSplitStepMs = 100;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        CenterSplitStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    m_strip.clear();

    const std::uint8_t leftLed =
        (LedCount / 2) - 1 - m_effectPhase;

    const std::uint8_t rightLed =
        (LedCount / 2) + m_effectPhase;

    m_strip.setPixelColor(
        leftLed,
        red,
        green,
        blue);

    m_strip.setPixelColor(
        rightLed,
        red,
        green,
        blue);

    m_strip.show();

    if (m_effectDirection > 0)
    {
        if (m_effectPhase >=
            ((LedCount / 2) - 1))
        {
            m_effectDirection = -1;
        }
        else
        {
            ++m_effectPhase;
        }
    }
    else
    {
        if (m_effectPhase == 0)
        {
            m_effectDirection = 1;
        }
        else
        {
            --m_effectPhase;
        }
    }
}

// -----------------------------------------------------------------------------
// Dual Chase
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateDualChase() noexcept
{
    /*
     * =========================================================
     * DUAL CHASE
     * =========================================================
     *
     *      3       4
     *      ↑       ↑
     *      2       5
     *      ↑       ↑
     *      1       6
     *
     * Ambos lados avanzan simultáneamente.
     */

    static constexpr std::uint32_t DualChaseStepMs = 100;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        DualChaseStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    m_strip.clear();

    const std::uint8_t leftLed =
        m_effectPhase;

    const std::uint8_t rightLed =
        (LedCount - 1) - m_effectPhase;

    m_strip.setPixelColor(
        leftLed,
        red,
        green,
        blue);

    m_strip.setPixelColor(
        rightLed,
        red,
        green,
        blue);

    m_strip.show();

    ++m_effectPhase;

    if (m_effectPhase >=
        (LedCount / 2))
    {
        m_effectPhase = 0;
    }
}

// -----------------------------------------------------------------------------
// Crossing
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateCrossing() noexcept
{
    /*
     * =========================================================
     * CROSSING
     * =========================================================
     *
     * Dos puntos recorren diagonales opuestas.
     *
     *      ●       ·
     *      ·       ●
     *      ·       ●
     *
     *      ↓
     *
     *      ·       ●
     *      ●       ·
     *      ·       ·
     */

    static constexpr std::uint32_t CrossingStepMs = 120;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        CrossingStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    static constexpr std::uint8_t leftPath[] =
    {
        0, 1, 2
    };

    static constexpr std::uint8_t rightPath[] =
    {
        5, 4, 3
    };

    const std::uint8_t phase =
        m_effectPhase % 3;

    m_strip.clear();

    m_strip.setPixelColor(
        leftPath[phase],
        red,
        green,
        blue);

    m_strip.setPixelColor(
        rightPath[phase],
        red,
        green,
        blue);

    m_strip.show();

    ++m_effectPhase;

    if (m_effectPhase >= 3)
    {
        m_effectPhase = 0;
    }
}

// -----------------------------------------------------------------------------
// Symmetric Pulse
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateSymmetricPulse() noexcept
{
    /*
     * =========================================================
     * SYMMETRIC PULSE
     * =========================================================
     *
     * Cada pareja física recibe un pulso de intensidad.
     *
     *      (1,6)
     *        ↓
     *      (2,5)
     *        ↓
     *      (3,4)
     */

    static constexpr std::uint32_t
        SymmetricPulseStepMs = 35;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        SymmetricPulseStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    const std::uint8_t pair =
        m_effectPhase / 4;

    const std::uint8_t pulsePhase =
        m_effectPhase % 4;

    std::uint8_t brightness = 0;

    switch (pulsePhase)
    {
    case 0:
        brightness = 80;
        break;

    case 1:
        brightness = 180;
        break;

    case 2:
        brightness = 255;
        break;

    case 3:
        brightness = 100;
        break;

    default:
        brightness = 0;
        break;
    }

    const std::uint8_t leftLed =
        pair;

    const std::uint8_t rightLed =
        (LedCount - 1) - pair;

    const std::uint16_t scaledRed =
        (static_cast<std::uint16_t>(red) *
         brightness) / 255;

    const std::uint16_t scaledGreen =
        (static_cast<std::uint16_t>(green) *
         brightness) / 255;

    const std::uint16_t scaledBlue =
        (static_cast<std::uint16_t>(blue) *
         brightness) / 255;

    m_strip.clear();

    m_strip.setPixelColor(
        leftLed,
        scaledRed,
        scaledGreen,
        scaledBlue);

    m_strip.setPixelColor(
        rightLed,
        scaledRed,
        scaledGreen,
        scaledBlue);

    m_strip.show();

    ++m_effectPhase;

    if (m_effectPhase >=
        ((LedCount / 2) * 4))
    {
        m_effectPhase = 0;
    }
}

// -----------------------------------------------------------------------------
// Center Pulse Wave
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateCenterPulseWave() noexcept
{
    /*
     * =========================================================
     * CENTER PULSE WAVE
     * =========================================================
     *
     *      (3,4)
     *        ↓
     *      (2,5)
     *        ↓
     *      (1,6)
     *
     * Las parejas anteriores permanecen iluminadas
     * con menor intensidad.
     */

    static constexpr std::uint32_t
        CenterPulseWaveStepMs = 70;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        CenterPulseWaveStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    const std::uint8_t center =
        LedCount / 2;

    m_strip.clear();

    for (std::uint8_t pair = 0;
         pair < center;
         ++pair)
    {
        const std::uint8_t distance =
            (center - 1) - pair;

        std::uint8_t brightness = 0;

        if (distance == m_effectPhase)
        {
            brightness = 255;
        }
        else if (distance + 1 ==
                 m_effectPhase)
        {
            brightness = 100;
        }
        else
        {
            brightness = 25;
        }

        const std::uint8_t leftLed =
            pair;

        const std::uint8_t rightLed =
            (LedCount - 1) - pair;

        const std::uint16_t scaledRed =
            (static_cast<std::uint16_t>(red) *
             brightness) / 255;

        const std::uint16_t scaledGreen =
            (static_cast<std::uint16_t>(green) *
             brightness) / 255;

        const std::uint16_t scaledBlue =
            (static_cast<std::uint16_t>(blue) *
             brightness) / 255;

        m_strip.setPixelColor(
            leftLed,
            scaledRed,
            scaledGreen,
            scaledBlue);

        m_strip.setPixelColor(
            rightLed,
            scaledRed,
            scaledGreen,
            scaledBlue);
    }

    m_strip.show();

    ++m_effectPhase;

    if (m_effectPhase >= center)
    {
        m_effectPhase = 0;
    }
}

// -----------------------------------------------------------------------------
// Side Chase
// -----------------------------------------------------------------------------

void AmbientLightController::UpdateSideChase() noexcept
{
    /*
     * =========================================================
     * SIDE CHASE
     * =========================================================
     *
     *      3       ·
     *      ·       5
     *      ·       ·
     *
     *      ·       4
     *      2       ·
     *
     *      1       6
     *
     * Cada lado avanza en dirección opuesta.
     */

    static constexpr std::uint32_t SideChaseStepMs = 100;

    const std::uint32_t now = millis();

    if ((now - m_lastEffectUpdate) <
        SideChaseStepMs)
    {
        return;
    }

    m_lastEffectUpdate = now;

    const std::uint32_t color =
        ColorToRgb(m_currentColor);

    const std::uint8_t red =
        (color >> 16) & 0xFF;

    const std::uint8_t green =
        (color >> 8) & 0xFF;

    const std::uint8_t blue =
        color & 0xFF;

    const std::uint8_t leftLed =
        m_effectPhase;

    const std::uint8_t rightLed =
        (LedCount - 1) - m_effectPhase;

    m_strip.clear();

    m_strip.setPixelColor(
        leftLed,
        red,
        green,
        blue);

    m_strip.setPixelColor(
        rightLed,
        red,
        green,
        blue);

    m_strip.show();

    ++m_effectPhase;

    if (m_effectPhase >=
        (LedCount / 2))
    {
        m_effectPhase = 0;
    }
}

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
