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

#include "Types/Vehicle/Direction.h"
#include "Types/Vehicle/Steering.h"
#include "Types/Vehicle/Turbo.h"
#include "Types/Vehicle/DriveMode.h"

namespace MK
{

//=============================================================================
// Alias
//=============================================================================

using Types::Vehicle::Direction;
using Types::Vehicle::Steering;
using Types::Vehicle::Turbo;
using Types::Vehicle::DriveMode;

//=============================================================================
// Constantes
//=============================================================================

inline constexpr char UnknownText[] = "Unknown";

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
    }

    return UnknownText;
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
    }

    return UnknownText;
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
            return "Disabled";

        case Turbo::Enabled:
            return "Enabled";
    }

    return UnknownText;
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
    }

    return UnknownText;
}

} // namespace MK

#endif // MK_SHARED_ENUMTOSTRING_H