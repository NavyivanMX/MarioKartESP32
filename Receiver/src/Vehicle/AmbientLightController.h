/******************************************************************************

* Proyecto : MarioKart ESP32 RC
* Archivo  : AmbientLightController.h
* Autor    : Narciso Ivan Cisneros Acosta
*
* Descripción:
* Controlador de iluminación ambiental del vehículo.
*
* Modo normal:
* * Muestra el color correspondiente al perfil de conducción.
* * Después de 10 segundos inicia el efecto Rainbow.
*
* Modo WTF:
* * Permite seleccionar manualmente color y efecto.
* * La última configuración WTF se conserva al salir del modo.
* * Al volver a entrar se recupera la configuración anterior.
*
* Colores:
* 
    Blue / Cyan / White / Red / Orange / Yellow
  
* 
    Green / Purple / Police / Rainbow / Off
  
*
* Efectos:
* 
    Solid / Fade / SequentialFade / AlternateFade
  
* 
    FastBlink / SlowBlink / Pulse
  
*

******************************************************************************/

#ifndef MK_AMBIENT_LIGHT_CONTROLLER_H
#define MK_AMBIENT_LIGHT_CONTROLLER_H

#include <cstdint>
#include <Adafruit_NeoPixel.h>

#include "src/Vehicle/VehicleProfiles/DrivingProfileManager.h"

namespace MK
{

class AmbientLightController final
{
public:


enum class AmbientColor : std::uint8_t
{
    Blue = 0,
    Cyan,
    White,
    Red,
    Orange,
    Yellow,
    Green,
    Purple,
    Police,
    Rainbow,
    Off,
    Count
};

enum class AmbientEffect : std::uint8_t
{
    Solid = 0,
    Fade,
    SequentialFade,
    AlternateFade,
    FastBlink,
    SlowBlink,
    Pulse,
    Count
};


public:


bool Begin() noexcept;

void Update() noexcept;

void SetProfile(
    VehicleProfiles::DrivingProfileId profile) noexcept;

void ToggleWtfMode() noexcept;

[[nodiscard]]
bool IsWtfMode() const noexcept;

void NextColor() noexcept;
void PreviousColor() noexcept;

void NextEffect() noexcept;
void PreviousEffect() noexcept;

[[nodiscard]]
AmbientColor CurrentColor() const noexcept;

[[nodiscard]]
AmbientEffect CurrentEffect() const noexcept;

void Off() noexcept;

void SetEnabled(bool enabled) noexcept;

[[nodiscard]]
bool IsEnabled() const noexcept;


private:


void UpdateNormalMode() noexcept;
void UpdateWtfMode() noexcept;

void UpdateSolid() noexcept;
void UpdateFade() noexcept;
void UpdateSequentialFade() noexcept;
void UpdateAlternateFade() noexcept;
void UpdateFastBlink() noexcept;
void UpdateSlowBlink() noexcept;
void UpdatePulse() noexcept;

void UpdatePolice() noexcept;
void UpdateRainbow() noexcept;

void ShowColor(
    std::uint8_t red,
    std::uint8_t green,
    std::uint8_t blue) noexcept;

void ShowAmbientColor(
    AmbientColor color) noexcept;

void ShowCurrentConfiguration() noexcept;

void StartWtfConfirmationBlink() noexcept;

void UpdateWtfConfirmationBlink() noexcept;

[[nodiscard]]
std::uint32_t ColorToRgb(
    AmbientColor color) const noexcept;

[[nodiscard]]
AmbientColor NextNormalColor(
    AmbientColor color) const noexcept;

[[nodiscard]]
AmbientColor PreviousNormalColor(
    AmbientColor color) const noexcept;

[[nodiscard]]
AmbientEffect NextEffectValue(
    AmbientEffect effect) const noexcept;

[[nodiscard]]
AmbientEffect PreviousEffectValue(
    AmbientEffect effect) const noexcept;

[[nodiscard]]
std::uint32_t Wheel(
    std::uint8_t position) const noexcept;


private:


static constexpr std::uint8_t LedCount = 4;
static constexpr std::uint8_t LedPin = 19;
static constexpr std::uint8_t Brightness = 100;

static constexpr std::uint32_t ProfileDisplayTimeMs = 10000;

static constexpr std::uint32_t RainbowStepMs = 20;

static constexpr std::uint32_t FadeStepMs = 10;

static constexpr std::uint32_t FastBlinkIntervalMs = 150;
static constexpr std::uint32_t SlowBlinkIntervalMs = 500;

static constexpr std::uint32_t PulseStepMs = 10;

static constexpr std::uint32_t WtfConfirmationBlinkIntervalMs = 180;
static constexpr std::uint8_t WtfConfirmationBlinkCount = 2;


private:


Adafruit_NeoPixel m_strip{
    LedCount,
    LedPin,
    NEO_GRB + NEO_KHZ800
};

VehicleProfiles::DrivingProfileId m_profile{
    VehicleProfiles::DrivingProfileId::Rookie
};

bool m_enabled{true};
bool m_wtfMode{false};

/*
 * Indica que el usuario terminó de configurar el ambiente
 * y desea mantener esa configuración fuera del modo WTF.
 *
 * WTF es el modo de configuración.
 * Custom Ambient es el modo de ejecución de la configuración.
 */
bool m_customAmbientActive{false};

/*
 * Configuración WTF actualmente seleccionada.
 */
AmbientColor m_currentColor{AmbientColor::Blue};
AmbientEffect m_currentEffect{AmbientEffect::Solid};

/*
 * Última configuración WTF guardada.
 *
 * Estas variables NO se modifican cuando se cambia de perfil.
 * Solamente cambian cuando el usuario modifica la configuración
 * estando dentro de WTF.
 */
AmbientColor m_savedWtfColor{AmbientColor::Blue};
AmbientEffect m_savedWtfEffect{AmbientEffect::Solid};

/*
 * Estado del modo normal.
 */
bool m_profileDisplayActive{true};
std::uint32_t m_profileStartedAt{0};

/*
 * Rainbow.
 */
std::uint8_t m_rainbowOffset{0};
std::uint32_t m_lastRainbowUpdate{0};

/*
 * Efectos.
 */
std::uint32_t m_lastEffectUpdate{0};
std::uint8_t m_effectPhase{0};
bool m_effectState{false};

/*
 * Fade / Pulse.
 */
std::int16_t m_effectBrightness{0};
std::int8_t m_effectDirection{1};

/*
 * Confirmación visual al entrar/salir de WTF.
 */
bool m_wtfConfirmationActive{false};
std::uint8_t m_wtfConfirmationCount{0};
bool m_wtfConfirmationState{false};
std::uint32_t m_wtfConfirmationStartedAt{0};

/*
 * Rainbow especial.
 */
bool m_rainbowActive{false};


};

}

#endif
