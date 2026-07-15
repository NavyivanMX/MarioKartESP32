/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotionController.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador del movimiento del vehículo.
 * Delega el procesamiento del DriverCommand al MotorController.
 ******************************************************************************/

#ifndef MK_RECEIVER_MOTIONCONTROLLER_H
#define MK_RECEIVER_MOTIONCONTROLLER_H

#include <MKShared.h>

#include "src/Vehicle/MotorController.h"

namespace MK
{

class MotionController final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    /// Inicializa el subsistema de movimiento.
    [[nodiscard]]
    bool Begin() noexcept;

    //=========================================================================
    // Movimiento
    //=========================================================================

    /// Actualiza el movimiento del vehículo.
    void Update(
        const Protocol::DriverCommand& command) noexcept;

private:

    //=========================================================================
    // Subsistemas
    //=========================================================================

    MotorController m_motor;
};

} // namespace MK

#endif // MK_RECEIVER_MOTIONCONTROLLER_H