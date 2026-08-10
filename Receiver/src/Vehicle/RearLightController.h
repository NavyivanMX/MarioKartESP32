/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : RearLightController.h
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
 *
 * En estado normal las luces permanecen apagadas.
 ******************************************************************************/

#ifndef MK_REAR_LIGHT_CONTROLLER_H
#define MK_REAR_LIGHT_CONTROLLER_H

//=============================================================================
// Includes
//=============================================================================

#include <cstdint>

#include <Adafruit_NeoPixel.h>

#include <Protocol/Protocol.h>

#include "src/Vehicle/VehicleProfiles/DrivingProfile.h"

namespace MK
{

class RearLightController final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    RearLightController();

    [[nodiscard]]
    bool Begin() noexcept;

    //=========================================================================
    // Ejecución
    //=========================================================================

    void Update(
        const Protocol::DriverCommand& command,
        const VehicleProfiles::DrivingProfile& profile) noexcept;

    //=========================================================================
    // Stop
    //=========================================================================

    void Stop() noexcept;

private:

    //=========================================================================
    // Hardware
    //=========================================================================

    static constexpr std::uint8_t LedPin = 4;

    static constexpr std::uint8_t LedCount = 2;

    static constexpr std::uint8_t Brightness = 100;

    Adafruit_NeoPixel m_strip;

    //=========================================================================
    // Estado general
    //=========================================================================

    Protocol::DriverCommand m_lastCommand{};

    bool m_hasLastCommand = false;

    //=========================================================================
    // Perfil
    //=========================================================================

    const char* m_lastProfileName = nullptr;

    //=========================================================================
    // Startup
    //=========================================================================

    bool m_startupAnimationActive = false;

    std::uint8_t m_startupBlinkCount = 0;

    bool m_startupBlinkOn = false;

    std::uint32_t m_startupBlinkLastUpdate = 0;

    //=========================================================================
    // Cambio de perfil
    //=========================================================================

    bool m_profileAnimationActive = false;

    std::uint8_t m_profileBlinkCount = 0;

    bool m_profileBlinkOn = false;

    std::uint32_t m_profileBlinkLastUpdate = 0;

    //=========================================================================
    // Turbo
    //=========================================================================

    bool m_turboActive = false;

    std::uint32_t m_turboLastUpdate = 0;

    std::uint16_t m_turboPhase = 0;

    //=========================================================================
    // Animaciones
    //=========================================================================

    void StartStartupAnimation() noexcept;

    void UpdateStartupAnimation(
        const VehicleProfiles::DrivingProfile& profile) noexcept;

    void StartProfileAnimation(
        const VehicleProfiles::DrivingProfile& profile) noexcept;

    void UpdateProfileAnimation(
        const VehicleProfiles::DrivingProfile& profile) noexcept;

    void UpdateTurboEffect() noexcept;

    //=========================================================================
    // Perfil
    //=========================================================================

    void GetProfileColor(
        const VehicleProfiles::DrivingProfile& profile,
        std::uint8_t& red,
        std::uint8_t& green,
        std::uint8_t& blue) const noexcept;

    //=========================================================================
    // Efectos básicos
    //=========================================================================

    void SetWhite() noexcept;

    void SetColor(
        std::uint8_t red,
        std::uint8_t green,
        std::uint8_t blue) noexcept;

    void Clear() noexcept;
};

} // namespace MK

#endif // MK_REAR_LIGHT_CONTROLLER_H