/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : EnumToString.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Funciones utilitarias para convertir enumeraciones del proyecto
 * a cadenas de texto legibles.
 ******************************************************************************/

#ifndef MK_SHARED_ENUMTOSTRING_H
#define MK_SHARED_ENUMTOSTRING_H

#include "Types/Types.h"

namespace MK
{

// Alias locales para mejorar legibilidad
using Types::Vehicle::Direction;
using Types::Vehicle::Steering;
using Types::Vehicle::Turbo;
using Types::Vehicle::DriveMode;

//=============================================================================
// Direction
//=============================================================================

[[nodiscard]]
inline constexpr const char* ToString(Direction direction) noexcept
{
    switch (direction)
    {
        case Direction::Stop:
            return "Stop";

        case Direction::Forward:
            return "Forward";

        case Direction::Reverse:
            return "Reverse";

        default:
            return "Unknown";
    }
}

//=============================================================================
// Steering
//=============================================================================

[[nodiscard]]
inline constexpr const char* ToString(Steering steering) noexcept
{
    switch (steering)
    {
        case Steering::Straight:
            return "Straight";

        case Steering::Left:
            return "Left";

        case Steering::Right:
            return "Right";

        default:
            return "Unknown";
    }
}

//=============================================================================
// Turbo
//=============================================================================

[[nodiscard]]
inline constexpr const char* ToString(Turbo turbo) noexcept
{
    switch (turbo)
    {
        case Turbo::Disabled:
            return "OFF";

        case Turbo::Enabled:
            return "ON";

        default:
            return "Unknown";
    }
}

//=============================================================================
// DriveMode
//=============================================================================

[[nodiscard]]
inline constexpr const char* ToString(DriveMode mode) noexcept
{
    switch (mode)
    {
        case DriveMode::Normal:
            return "Normal";

        case DriveMode::Gravity:
            return "Gravity";

        default:
            return "Unknown";
    }
}

} // namespace MK

#endif // MK_SHARED_ENUMTOSTRING_H