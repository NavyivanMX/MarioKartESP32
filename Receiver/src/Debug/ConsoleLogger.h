/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ConsoleLogger.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Logger de consola del Receiver.
 * Centraliza todos los mensajes de depuración y diagnóstico del sistema.
 ******************************************************************************/

#ifndef MK_RECEIVER_CONSOLELOGGER_H
#define MK_RECEIVER_CONSOLELOGGER_H

//=============================================================================
// Includes
//=============================================================================

#include <MKShared.h>

#include "src/Vehicle/VehicleProfiles/DrivingProfile.h"

namespace MK
{

class ConsoleLogger final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    void Begin() noexcept;

    //=========================================================================
    // Sistema
    //=========================================================================

    void LogBoot() const noexcept;

    void LogWiFiMac() const noexcept;

    void LogBluetooth(
        const char* deviceName) const noexcept;

    void LogReady() const noexcept;

    void LogError(
        const char* message) const noexcept;

    //=========================================================================
    // Driving Profile
    //=========================================================================

    void LogProfile(
        const VehicleProfiles::DrivingProfile& profile) const noexcept;

    //=========================================================================
    // DriverCommand
    //=========================================================================

    void Log(
        const Protocol::DriverCommand& command) const noexcept;

private:

    [[nodiscard]]
    static const char* ToString(
        Types::Vehicle::Direction direction) noexcept;

    [[nodiscard]]
    static const char* ToString(
        Types::Vehicle::Steering steering) noexcept;

    [[nodiscard]]
    static const char* ToString(
        Types::Vehicle::Turbo turbo) noexcept;

    [[nodiscard]]
    static const char* ToString(
        Types::Vehicle::DriveMode mode) noexcept;
};

} // namespace MK

#endif