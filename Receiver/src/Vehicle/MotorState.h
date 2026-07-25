/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotorState.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Estado de un motor del vehículo.
 * Representa la orden que el MotorController envía al MotorDriver.
 ******************************************************************************/

#ifndef MK_RECEIVER_MOTORSTATE_H
#define MK_RECEIVER_MOTORSTATE_H

#include <cstdint>

#include <MKShared.h>

namespace MK
{

struct MotorState
{
    //=========================================================================
    // Movimiento
    //=========================================================================

    /// Dirección de giro del motor.
    Types::Vehicle::Direction direction =
        Types::Vehicle::Direction::Stop;

    /// Nivel de potencia solicitado.
    ///
    /// Rango:
    ///     0   = Motor detenido.
    ///     255 = Máxima potencia.
    ///
    /// El MotorDriver será el responsable de traducir este valor al
    /// mecanismo físico correspondiente (PWM mediante LEDC).
    std::uint8_t power = 0;
};

} // namespace MK

#endif // MK_RECEIVER_MOTORSTATE_H