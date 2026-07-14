/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Direction.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Define la dirección de desplazamiento del vehículo.
 ******************************************************************************/

#ifndef MK_SHARED_DIRECTION_H
#define MK_SHARED_DIRECTION_H

#include <cstdint>

namespace MK::Types::Vehicle
{

//=============================================================================
// Direction
//=============================================================================

/// Dirección de desplazamiento del vehículo.
enum class Direction : std::uint8_t
{
    Stop = 0,
    Forward,
    Reverse
};

} // namespace MK::Types::Vehicle

#endif // MK_SHARED_DIRECTION_H