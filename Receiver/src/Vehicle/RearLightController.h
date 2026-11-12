#ifndef MK_REAR_LIGHT_CONTROLLER_H
#define MK_REAR_LIGHT_CONTROLLER_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include <Protocol/Protocol.h>

#include "src/Vehicle/VehicleProfiles/DrivingProfile.h"

namespace MK
{

class RearLightController final
{
public:

    [[nodiscard]]
    bool Begin(std::uint8_t ledCount = 2) noexcept;

    void Update(
        const Protocol::DriverCommand& command,
        const VehicleProfiles::DrivingProfile& profile) noexcept;

    void Stop() noexcept;

private:

    void UpdateStartupEffect() noexcept;

    void UpdateProfileEffect(
        const VehicleProfiles::DrivingProfile& profile) noexcept;

    void UpdatePoliceProfileEffect() noexcept;

    void UpdateTurboEffect(
        const VehicleProfiles::DrivingProfile& profile) noexcept;

    void UpdatePoliceTurboEffect() noexcept;

    bool IsPoliceProfile(
        const VehicleProfiles::DrivingProfile& profile) const noexcept;

    void GetProfileColor(
        const VehicleProfiles::DrivingProfile& profile,
        std::uint8_t& red,
        std::uint8_t& green,
        std::uint8_t& blue) const noexcept;

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

private:

    static constexpr std::uint8_t DefaultLedCount = 2;
    static constexpr std::uint8_t LedPin = 4;

    std::uint8_t m_ledCount{DefaultLedCount};

    Adafruit_NeoPixel m_strip{
        DefaultLedCount,
        LedPin,
        NEO_GRB + NEO_KHZ800
    };

    bool m_started = false;

    const char* m_lastProfileName = nullptr;
    bool m_hasLastProfile = false;

    bool m_startupActive = true;
    bool m_startupBlinkOn = false;
    std::uint8_t m_startupBlinkCount = 0;
    std::uint32_t m_startupLastUpdate = 0;

    bool m_profileEffectActive = false;
    bool m_profileBlinkOn = false;
    std::uint8_t m_profileBlinkCount = 0;
    std::uint32_t m_profileLastUpdate = 0;

    std::uint16_t m_turboPhase = 0;
    std::uint32_t m_turboLastUpdate = 0;

    bool m_policeTurboState = false;
    std::uint32_t m_policeTurboLastUpdate = 0;
};

}

#endif
