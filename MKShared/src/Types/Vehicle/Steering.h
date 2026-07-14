/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Steering.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Define la dirección de desplazamiento del vehículo.
 ******************************************************************************/

#ifndef MK_SHARED_STEERING_H
#define MK_SHARED_STEERING_H

#include <cstdint>

namespace MK::Types::Vehicle
{

//=============================================================================
// Steering
//=============================================================================

/// Dirección de desplazamiento del vehículo.
enum class Steering : std::uint8_t
{
    Straight = 0,
    Left,
    Right
};

} // namespace MK::Types::Vehicle

#endif // MK_SHARED_STEERING_H