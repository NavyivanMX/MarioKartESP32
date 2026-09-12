/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : AmbientLightController.h
 *
 * Ambiente:
 *   4 x WS2812B
 *   GPIO 19
 *
 * Modo NORMAL:
 *   Al cambiar de perfil:
 *   1. Color correspondiente al perfil.
 *   2. Mantiene el color durante 10 segundos.
 *   3. Entra en Rainbow Shift continuo.
 *
 * Modo WTF:
 *   Se activa/desactiva mediante Perfil + Turbo durante 2 segundos.
 *
 *   Perfil + Adelante/Atrás:
 *       Cambia COLOR.
 *
 *       Colores:
 *         - Azul
 *         - Azul claro
 *         - Blanco
 *         - Rojo
 *         - Naranja
 *         - Amarillo
 *         - Verde
 *         - Morado
 *         - Policía
 *         - Apagado
 *
 *   Perfil + Derecha/Izquierda:
 *       Cambia EFECTO.
 *
 *       Efectos:
 *         - Fijo
 *         - Fade parejo
 *         - Fade secuencial
 *         - Fade alterno
 *         - Blink rápido
 *         - Blink lento
 *         - Pulse
 *
 *   Apagado:
 *       No utiliza ningún efecto.
 *
 * El controlador es NO BLOQUEANTE:
 *   no utiliza delay() en sus efectos.
 ******************************************************************************/

#ifndef MK_AMBIENT_LIGHT_CONTROLLER_H
#define MK_AMBIENT_LIGHT_CONTROLLER_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include <Vehicle/VehicleProfiles/DrivingProfileId.h>

namespace MK
{

class AmbientLightController final
{
public:

    //=====================================================================
    // Enumeraciones
    //=====================================================================

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

    //=====================================================================
    // Inicialización / actualización
    //=====================================================================

    bool Begin() noexcept;

    void Update() noexcept;

    //=====================================================================
    // Modo NORMAL
    //=====================================================================

    // Inicia/reinicia la secuencia del ambiente para el perfil indicado.
    //
    // Perfil
    //   ↓
    // Color del perfil
    //   ↓
    // 10 segundos
    //   ↓
    // Rainbow
    //
    void SetProfile(
        VehicleProfiles::DrivingProfileId profile) noexcept;

    //=====================================================================
    // Modo WTF
    //=====================================================================

    // Activa/desactiva el modo WTF.
    void ToggleWtfMode() noexcept;

    [[nodiscard]]
    bool IsWtfMode() const noexcept;

    //=====================================================================
    // Selección WTF - COLOR
    //=====================================================================

    // Selecciona el siguiente color.
    //
    // Blue -> Cyan -> White -> ... -> Police -> Off -> Blue
    //
    void NextColor() noexcept;

    // Selecciona el color anterior.
    void PreviousColor() noexcept;

    //=====================================================================
    // Selección WTF - EFECTO
    //=====================================================================

    // Selecciona el siguiente efecto.
    //
    // Solid -> Fade -> SequentialFade -> ... -> Pulse -> Solid
    //
    void NextEffect() noexcept;

    // Selecciona el efecto anterior.
    void PreviousEffect() noexcept;

    //=====================================================================
    // Estado
    //=====================================================================

    [[nodiscard]]
    AmbientColor CurrentColor() const noexcept;

    [[nodiscard]]
    AmbientEffect CurrentEffect() const noexcept;

    //=====================================================================
    // ON / OFF
    //=====================================================================

    // Desactiva completamente el ambiente.
    void Off() noexcept;

    //=====================================================================
    // Configuración general
    //=====================================================================

    void SetEnabled(bool enabled) noexcept;

    [[nodiscard]]
    bool IsEnabled() const noexcept;

private:

    //=====================================================================
    // Hardware
    //=====================================================================

    static constexpr std::uint8_t LedPin = 19;

    static constexpr std::uint8_t LedCount = 4;

    static constexpr std::uint8_t Brightness = 100;

    //=====================================================================
    // Tiempos
    //=====================================================================

    // Tiempo que permanece el color del perfil antes del Rainbow.
    static constexpr std::uint32_t ProfileColorTimeMs = 10000;

    // Velocidad del Rainbow Shift.
    static constexpr std::uint32_t RainbowStepMs = 20;

    //=====================================================================
    // Hardware
    //=====================================================================

    Adafruit_NeoPixel m_strip{
        LedCount,
        LedPin,
        NEO_GRB + NEO_KHZ800};

    //=====================================================================
    // Estado general
    //=====================================================================

    bool m_started{false};

    bool m_enabled{true};

    // true = estamos dentro del modo WTF.
    bool m_wtfMode{false};

    // true = ya terminó la etapa de 10 segundos
    // y actualmente estamos en Rainbow.
    bool m_rainbowActive{false};

    //=====================================================================
    // Perfil actual
    //=====================================================================

    VehicleProfiles::DrivingProfileId m_profile{
        VehicleProfiles::DrivingProfileId::Normal};

    //=====================================================================
    // Selección WTF
    //=====================================================================

    AmbientColor m_currentColor{
        AmbientColor::Blue};

    AmbientEffect m_currentEffect{
        AmbientEffect::Solid};

    //=====================================================================
    // Temporizadores
    //=====================================================================

    std::uint32_t m_profileStartedAt{0};

    std::uint32_t m_lastRainbowUpdate{0};

    //=====================================================================
    // Rainbow
    //=====================================================================

    std::uint16_t m_rainbowOffset{0};

    //=====================================================================
    // Colores
    //=====================================================================

    [[nodiscard]]
    std::uint32_t ColorForProfile(
        VehicleProfiles::DrivingProfileId profile) const noexcept;

    [[nodiscard]]
    std::uint32_t ColorForAmbient(
        AmbientColor color) const noexcept;

    //=====================================================================
    // Rainbow
    //=====================================================================

    [[nodiscard]]
    std::uint32_t Wheel(
        std::uint8_t position) const noexcept;

    void ShowRainbow() noexcept;

    //=====================================================================
    // WTF - efectos
    //=====================================================================

    void ShowWtf() noexcept;

    void ShowSolid() noexcept;

    void ShowFade() noexcept;

    void ShowSequentialFade() noexcept;

    void ShowAlternateFade() noexcept;

    void ShowFastBlink() noexcept;

    void ShowSlowBlink() noexcept;

    void ShowPulse() noexcept;

    void ShowPolice() noexcept;

    //=====================================================================
    // Utilidades
    //=====================================================================

    void SetAll(
        std::uint32_t color) noexcept;

    void ShowProfileColor() noexcept;

    void ShowCurrentColor() noexcept;

    void ShowCurrentEffect() noexcept;

    // Ajusta brillo individual de un LED.
    void SetPixelScaled(
        std::uint8_t led,
        std::uint8_t red,
        std::uint8_t green,
        std::uint8_t blue,
        std::uint8_t scale) noexcept;

    // Onda triangular 0 -> 255 -> 0.
    [[nodiscard]]
    static std::uint8_t TriangularWave(
        std::uint16_t phase) noexcept;
};

} // namespace MK

#endif // MK_AMBIENT_LIGHT_CONTROLLER_H
