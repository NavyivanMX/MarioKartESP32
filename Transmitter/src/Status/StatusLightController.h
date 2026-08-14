/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : StatusLightController.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador de la luz de estado del transmisor.
 ******************************************************************************/

#ifndef MK_STATUS_LIGHT_CONTROLLER_H
#define MK_STATUS_LIGHT_CONTROLLER_H

//=============================================================================
// Includes
//=============================================================================

#include <cstdint>

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include <MKShared.h>

#include "src/Config/Pins.h"

namespace MK
{

//=============================================================================
// StatusLightController
//=============================================================================

class StatusLightController final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    [[nodiscard]]
    bool Begin() noexcept;

    //=========================================================================
    // Estado
    //=========================================================================

    void Update(
        const Protocol::DriverCommand& command,
        VehicleProfiles::DrivingProfileId profile) noexcept;

    //=========================================================================
    // Control
    //=========================================================================

    void Stop() noexcept;

private:

    //=========================================================================
    // Efectos
    //=========================================================================

    void UpdateStartupEffect() noexcept;

    void UpdateProfileEffect() noexcept;

    void UpdateTurboEffect() noexcept;

    void UpdatePoliceTurboEffect() noexcept;

    //=========================================================================
    // Perfil
    //=========================================================================

    void UpdateProfile(
        VehicleProfiles::DrivingProfileId profile) noexcept;

    [[nodiscard]]
    bool IsPoliceProfile(
        VehicleProfiles::DrivingProfileId profile) const noexcept;

    void GetProfileColor(
        VehicleProfiles::DrivingProfileId profile,
        std::uint8_t& red,
        std::uint8_t& green,
        std::uint8_t& blue) const noexcept;

    //=========================================================================
    // LED
    //=========================================================================

    void SetColor(
        std::uint8_t red,
        std::uint8_t green,
        std::uint8_t blue) noexcept;

    void SetWhite() noexcept;

    void Clear() noexcept;

    //=========================================================================
    // Hardware
    //=========================================================================

    static constexpr std::uint8_t LedCount = 1;

    Adafruit_NeoPixel m_strip{
        LedCount,
        Pins::StatusLed,
        NEO_GRB + NEO_KHZ800
    };

    //=========================================================================
    // Estado general
    //=========================================================================

    bool m_started = false;

    //=========================================================================
    // Perfil
    //=========================================================================

    VehicleProfiles::DrivingProfileId
        m_currentProfile =
            VehicleProfiles::DrivingProfileId::Rookie;

    VehicleProfiles::DrivingProfileId
        m_lastProfile =
            VehicleProfiles::DrivingProfileId::Rookie;

    bool m_hasProfile = false;

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
    // Turbo
    //=========================================================================

    bool m_turboActive = false;

    std::uint16_t m_turboPhase = 0;

    std::uint32_t m_turboLastUpdate = 0;

    //=========================================================================
    // Turbo Police
    //=========================================================================

    bool m_policeTurboState = false;

    std::uint32_t m_policeTurboLastUpdate = 0;
};

} // namespace MK

#endif // MK_STATUS_LIGHT_CONTROLLER_H