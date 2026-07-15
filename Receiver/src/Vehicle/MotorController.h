/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotorController.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador del sistema de propulsión.
 * Interpreta el DriverCommand y determina el estado de cada motor.
 ******************************************************************************/

#ifndef MK_RECEIVER_MOTORCONTROLLER_H
#define MK_RECEIVER_MOTORCONTROLLER_H

#include <MKShared.h>

#include "src/Drivers/MotorDriver.h"
#include "src/Vehicle/MotorState.h"

namespace MK
{

class MotorController final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    /// Inicializa el subsistema de propulsión.
    [[nodiscard]]
    bool Begin() noexcept;

    //=========================================================================
    // Movimiento
    //=========================================================================

    /// Procesa el comando recibido y actualiza los motores.
    void Drive(
        const Protocol::DriverCommand& command) noexcept;

private:

    //=========================================================================
    // Aplicación al hardware
    //=========================================================================

    /// Envía el estado calculado a ambos motores.
    void Apply(
        const MotorState& left,
        const MotorState& right) noexcept;

private:

    //=========================================================================
    // Drivers
    //=========================================================================

    MotorDriver m_driver;
};

} // namespace MK

#endif // MK_RECEIVER_MOTORCONTROLLER_H