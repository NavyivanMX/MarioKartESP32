/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : RearLightController.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador de las luces traseras del kart.
 ******************************************************************************/

#ifndef MK_REAR_LIGHT_CONTROLLER_H
#define MK_REAR_LIGHT_CONTROLLER_H

//=============================================================================
// Includes
//=============================================================================

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include <Protocol/Protocol.h>

#include "src/Vehicle/VehicleProfiles/DrivingProfile.h"

namespace MK
{

//=============================================================================
// RearLightController
//=============================================================================

class RearLightController final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    [[nodiscard]]
    bool Begin() noexcept;

    //=========================================================================
    // Ejecución
    //=========================================================================

    void Update(
        const Protocol::DriverCommand& command,
        const VehicleProfiles::DrivingProfile& profile) noexcept;

    //=========================================================================
    // Control
    //=========================================================================

    void Stop() noexcept;

private:

    //=========================================================================
    // Efectos
    //=========================================================================

    void UpdateStartupEffect() noexcept;

    void UpdateProfileEffect(
        const VehicleProfiles::DrivingProfile& profile) noexcept;

    void UpdatePoliceProfileEffect() noexcept;

    void UpdateTurboEffect(
        const VehicleProfiles::DrivingProfile& profile) noexcept;

    void UpdatePoliceTurboEffect() noexcept;

    //=========================================================================
    // Perfiles
    //=========================================================================

    bool IsPoliceProfile(
        const VehicleProfiles::DrivingProfile& profile) const noexcept;

    void GetProfileColor(
        const VehicleProfiles::DrivingProfile& profile,
        std::uint8_t& red,
        std::uint8_t& green,
        std::uint8_t& blue) const noexcept;

    //=========================================================================
    // LEDs
    //=========================================================================

    void SetColor(
        std::uint8_t red,
        std::uint8_t green,
        std::uint8_t blue) noexcept;

    void SetLedColor(
        std::uint8_t index,
        std::uint8_t red,
        std::uint8_t green,
        std::uint8_t blue) noexcept;

    void SetWhite() noexcept;

    void Clear() noexcept;

    //=========================================================================
    // Hardware
    //=========================================================================

    static constexpr std::uint8_t LedPin = 4;

    static constexpr std::uint8_t LedCount = 2;

    Adafruit_NeoPixel m_strip{
        LedCount,
        LedPin,
        NEO_GRB + NEO_KHZ800
    };

    //=========================================================================
    // Estado general
    //=========================================================================

    bool m_started = false;

    //=========================================================================
    // Perfil
    //=========================================================================

    const char* m_lastProfileName = nullptr;

    bool m_hasLastProfile = false;

    //=========================================================================
    // Startup
    //=========================================================================

    bool m_startupActive = true;

    bool m_startupBlinkOn = false;

    std::uint8_t m_startupBlinkCount = 0;

    std::uint32_t m_startupLastUpdate = 0;

    //=========================================================================
    // Cambio de perfil
    //=========================================================================

    bool m_profileEffectActive = false;

    bool m_profileBlinkOn = false;

    std::uint8_t m_profileBlinkCount = 0;

    std::uint32_t m_profileLastUpdate = 0;

    //=========================================================================
    // Turbo normal
    //=========================================================================

    std::uint16_t m_turboPhase = 0;

    std::uint32_t m_turboLastUpdate = 0;

    //=========================================================================
    // Turbo Police
    //=========================================================================

    bool m_policeTurboState = false;

    std::uint32_t m_policeTurboLastUpdate = 0;
};

} // namespace MK

#endif // MK_REAR_LIGHT_CONTROLLER_H