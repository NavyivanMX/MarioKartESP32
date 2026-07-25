/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotorController.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador del sistema de propulsión.
 *
 * Interpreta el DriverCommand recibido y calcula el estado individual
 * de cada motor (dirección y potencia), delegando la aplicación física
 * al MotorDriver.
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

    /// Inicializa el sistema de propulsión.
    [[nodiscard]]
    bool Begin() noexcept;

    //=========================================================================
    // Movimiento
    //=========================================================================

    /// Procesa el comando recibido y calcula el estado de ambos motores.
    void Drive(
        const Protocol::DriverCommand& command) noexcept;

private:

    //=========================================================================
    // Hardware
    //=========================================================================

    /// Envía el estado calculado al driver de hardware.
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