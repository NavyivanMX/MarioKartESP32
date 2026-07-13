/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Protocol.h
 *
 * Descripción:
 * Define las estructuras utilizadas para la comunicación entre
 * Controller y Receiver mediante ESP-NOW.
 ******************************************************************************/

#ifndef MK_SHARED_PROTOCOL_H
#define MK_SHARED_PROTOCOL_H

#include <cstdint>

#include "src/Shared/Types.h"

namespace MK::Protocol
{

using Types::Vehicle::Direction;
using Types::Vehicle::Steering;
using Types::Vehicle::Turbo;
using Types::Vehicle::DriveMode;

//=============================================================================
// DriverCommand
//=============================================================================

struct DriverCommand
{
    Direction direction{Direction::Stop};

    Steering steering{Steering::Straight};

    Turbo turbo{Turbo::Disabled};

    DriveMode driveMode{DriveMode::Normal};

    constexpr bool operator==(const DriverCommand&) const noexcept = default;
};

} // namespace MK::Protocol

#endif // MK_SHARED_PROTOCOL_H