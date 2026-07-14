/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DriverCommandUtils.h
 *
 * Descripción:
 * Funciones auxiliares para trabajar con DriverCommand.
 ******************************************************************************/

#ifndef MK_SHARED_DRIVERCOMMANDUTILS_H
#define MK_SHARED_DRIVERCOMMANDUTILS_H

#include "Protocol/Protocol.h"

namespace MK
{
using Protocol::DriverCommand;
using Types::Vehicle::Direction;
using Types::Vehicle::Steering;
using Types::Vehicle::Turbo;
using Types::Vehicle::DriveMode;

//=============================================================================
// Estado del comando
//=============================================================================

/// Indica si el vehículo está en movimiento.
[[nodiscard]]
inline constexpr bool IsMoving(const DriverCommand& command) noexcept
{
    return command.direction != Direction::Stop;
}

//-----------------------------------------------------------------------------

/// Indica si el vehículo está detenido.
[[nodiscard]]
inline constexpr bool IsStopped(const DriverCommand& command) noexcept
{
    return command.direction == Direction::Stop;
}

//-----------------------------------------------------------------------------

/// Indica si el vehículo avanza.
[[nodiscard]]
inline constexpr bool IsForward(const DriverCommand& command) noexcept
{
    return command.direction == Direction::Forward;
}

//-----------------------------------------------------------------------------

/// Indica si el vehículo retrocede.
[[nodiscard]]
inline constexpr bool IsReverse(const DriverCommand& command) noexcept
{
    return command.direction == Direction::Reverse;
}

//-----------------------------------------------------------------------------

/// Indica si el vehículo está girando.
[[nodiscard]]
inline constexpr bool IsTurning(const DriverCommand& command) noexcept
{
    return command.steering != Steering::Straight;
}

//-----------------------------------------------------------------------------

/// Indica si las ruedas permanecen rectas.
[[nodiscard]]
inline constexpr bool IsStraight(const DriverCommand& command) noexcept
{
    return command.steering == Steering::Straight;
}

//-----------------------------------------------------------------------------

/// Indica si el Turbo está habilitado.
[[nodiscard]]
inline constexpr bool IsTurboEnabled(const DriverCommand& command) noexcept
{
    return command.turbo == Turbo::Enabled;
}

//-----------------------------------------------------------------------------

/// Indica si el modo Gravity está habilitado.
[[nodiscard]]
inline constexpr bool IsGravityMode(const DriverCommand& command) noexcept
{
    return command.driveMode == DriveMode::Gravity;
}

//=============================================================================
// Comparaciones
//=============================================================================

/// Indica si cualquier campo del comando cambió.
[[nodiscard]]
inline constexpr bool Changed(
    const DriverCommand& previous,
    const DriverCommand& current) noexcept
{
    return previous != current;
}

//-----------------------------------------------------------------------------

/// Indica si cambió la dirección.
[[nodiscard]]
inline constexpr bool DirectionChanged(
    const DriverCommand& previous,
    const DriverCommand& current) noexcept
{
    return previous.direction != current.direction;
}

//-----------------------------------------------------------------------------

/// Indica si cambió el giro.
[[nodiscard]]
inline constexpr bool SteeringChanged(
    const DriverCommand& previous,
    const DriverCommand& current) noexcept
{
    return previous.steering != current.steering;
}

//-----------------------------------------------------------------------------

/// Indica si cambió el estado del Turbo.
[[nodiscard]]
inline constexpr bool TurboChanged(
    const DriverCommand& previous,
    const DriverCommand& current) noexcept
{
    return previous.turbo != current.turbo;
}

//-----------------------------------------------------------------------------

/// Indica si cambió el modo de conducción.
[[nodiscard]]
inline constexpr bool DriveModeChanged(
    const DriverCommand& previous,
    const DriverCommand& current) noexcept
{
    return previous.driveMode != current.driveMode;
}

//=============================================================================
// Fábricas
//=============================================================================

/// Crea un DriverCommand con los valores por defecto.
[[nodiscard]]
inline constexpr DriverCommand MakeDefault() noexcept
{
    return DriverCommand{};
}

//-----------------------------------------------------------------------------

/// Crea un comando para detener completamente el vehículo.
[[nodiscard]]
inline constexpr DriverCommand MakeStop() noexcept
{
    return
    {
        Direction::Stop,
        Steering::Straight,
        Turbo::Disabled,
        DriveMode::Normal
    };
}

} // namespace MK

#endif // MK_SHARED_DRIVERCOMMANDUTILS_H