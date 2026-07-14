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

#include "Types/Vehicle/Direction.h"
#include "Types/Vehicle/Steering.h"
#include "Types/Vehicle/Turbo.h"
#include "Types/Vehicle/DriveMode.h"

namespace MK::Protocol
{

using Types::Vehicle::Direction;
using Types::Vehicle::Steering;
using Types::Vehicle::Turbo;
using Types::Vehicle::DriveMode;

//=============================================================================
// DriverCommand /// Comando enviado por el transmisor al vehículo.
//=============================================================================

struct DriverCommand
{
    Direction direction{Direction::Stop};

    Steering steering{Steering::Straight};

    Turbo turbo{Turbo::Disabled};

    DriveMode driveMode{DriveMode::Normal};

    constexpr bool operator==(const DriverCommand&) const noexcept = default;

    constexpr bool operator!=(const DriverCommand& other) const noexcept{
        return !(*this == other);
    }
};

static_assert(sizeof(DriverCommand) == 4,"DriverCommand size changed.");

} // namespace MK::Protocol

#endif // MK_SHARED_PROTOCOL_H