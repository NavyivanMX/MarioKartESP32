/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ProtocolUtils.h
 *
 * Descripción:
 * Funciones auxiliares relacionadas con DriverCommand.
 ******************************************************************************/

#ifndef MK_SHARED_PROTOCOLUTILS_H
#define MK_SHARED_PROTOCOLUTILS_H

#include "src/Shared/Protocol.h"

namespace MK::Protocol
{

//=============================================================================
// Estado
//=============================================================================

[[nodiscard]]
inline constexpr bool IsMoving(const DriverCommand& command) noexcept
{
    return command.direction != Direction::Stop;
}

//-----------------------------------------------------------------------------

[[nodiscard]]
inline constexpr bool IsTurning(const DriverCommand& command) noexcept
{
    return command.steering != Steering::Straight;
}

//-----------------------------------------------------------------------------

[[nodiscard]]
inline constexpr bool IsTurboEnabled(const DriverCommand& command) noexcept
{
    return command.turbo == Turbo::Enabled;
}

//-----------------------------------------------------------------------------

[[nodiscard]]
inline constexpr bool IsGravityMode(const DriverCommand& command) noexcept
{
    return command.driveMode == DriveMode::Gravity;
}

//=============================================================================
// Comparaciones
//=============================================================================

[[nodiscard]]
inline constexpr bool DirectionChanged(
    const DriverCommand& previous,
    const DriverCommand& current) noexcept
{
    return previous.direction != current.direction;
}

//-----------------------------------------------------------------------------

[[nodiscard]]
inline constexpr bool SteeringChanged(
    const DriverCommand& previous,
    const DriverCommand& current) noexcept
{
    return previous.steering != current.steering;
}

//-----------------------------------------------------------------------------

[[nodiscard]]
inline constexpr bool TurboChanged(
    const DriverCommand& previous,
    const DriverCommand& current) noexcept
{
    return previous.turbo != current.turbo;
}

//-----------------------------------------------------------------------------

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

[[nodiscard]]
inline constexpr DriverCommand MakeDefault() noexcept
{
    return DriverCommand{};
}

//-----------------------------------------------------------------------------

[[nodiscard]]
inline constexpr DriverCommand MakeStop() noexcept
{
    return DriverCommand
    {
        Direction::Stop,
        Steering::Straight,
        Turbo::Disabled,
        DriveMode::Normal
    };
}

} // namespace MK::Protocol

#endif // MK_SHARED_PROTOCOLUTILS_H