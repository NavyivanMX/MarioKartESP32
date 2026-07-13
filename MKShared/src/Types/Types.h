/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Types.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Define los tipos fundamentales compartidos por todo el proyecto.
 ******************************************************************************/

#ifndef MK_SHARED_TYPES_H
#define MK_SHARED_TYPES_H

#pragma once

#include <Vehicle/Direction.h>
#include <Vehicle/DriveMode.h>
#include <Vehicle/Steering.h>
#include <Vehicle/Turbo.h>

#include <array>
#include <cstddef>
#include <cstdint>

namespace MK::Types
{

//=============================================================================
// Alias
//=============================================================================

/// Longitud de una dirección MAC.
inline constexpr std::size_t MacAddressLength = 6;

/// Tipo que representa una dirección MAC.
using MacAddress = std::array<std::uint8_t, MacAddressLength>;

//=============================================================================
// Tipos del vehículo
//=============================================================================

namespace Vehicle
{

//-----------------------------------------------------------------------------
// Dirección de avance
//-----------------------------------------------------------------------------

enum class Direction : std::uint8_t
{
    Stop = 0,
    Forward,
    Reverse
};

//-----------------------------------------------------------------------------
// Dirección de giro
//-----------------------------------------------------------------------------

enum class Steering : std::uint8_t
{
    Straight = 0,
    Left,
    Right
};

//-----------------------------------------------------------------------------
// Estado del turbo
//-----------------------------------------------------------------------------

enum class Turbo : std::uint8_t
{
    Disabled = 0,
    Enabled
};

//-----------------------------------------------------------------------------
// Modo de conducción
//-----------------------------------------------------------------------------

enum class DriveMode : std::uint8_t
{
    Normal = 0,
    Gravity
};

} // namespace Vehicle

} // namespace MK::Types

#endif // MK_SHARED_TYPES_H