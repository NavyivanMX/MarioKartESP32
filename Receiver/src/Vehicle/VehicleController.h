/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : VehicleController.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador principal del vehículo.
 * Interpreta los comandos recibidos y coordina los diferentes subsistemas
 * del kart.
 ******************************************************************************/

#ifndef MK_RECEIVER_VEHICLECONTROLLER_H
#define MK_RECEIVER_VEHICLECONTROLLER_H

#include <MKShared.h>

#include "src/Vehicle/GravityController.h"
#include "src/Vehicle/MotionController.h"

namespace MK
{

class VehicleController final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    [[nodiscard]]
    bool Begin() noexcept;

    //=========================================================================
    // Control
    //=========================================================================

    void Update(
        const Protocol::DriverCommand& command) noexcept;

private:

    //=========================================================================
    // Subsistemas
    //=========================================================================

    MotionController m_motion;

    GravityController m_gravity;
};
} // namespace MK

#endif // MK_RECEIVER_VEHICLECONTROLLER_H