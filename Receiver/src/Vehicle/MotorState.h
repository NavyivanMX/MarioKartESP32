/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotorState.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Representa el estado deseado de un motor.
 ******************************************************************************/

#ifndef MK_RECEIVER_MOTORSTATE_H
#define MK_RECEIVER_MOTORSTATE_H

#include <MKShared.h>

namespace MK
{

struct MotorState
{
    /// Dirección de giro del motor.
    Types::Vehicle::Direction direction =
        Types::Vehicle::Direction::Stop;

    /// Modo de velocidad del motor.
    Types::Vehicle::Turbo turbo =
        Types::Vehicle::Turbo::Disabled;
};

} // namespace MK

#endif // MK_RECEIVER_MOTORSTATE_H