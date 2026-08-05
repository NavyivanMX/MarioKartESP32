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

//=============================================================================
// Includes
//=============================================================================

#include <MKShared.h>

#include "src/Drivers/MotorDriver.h"
#include "src/Vehicle/MotorState.h"

#include "src/Vehicle/VehicleProfiles/DrivingProfile.h"

namespace MK
{

class MotorController final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    [[nodiscard]]
    bool Begin() noexcept;

    //=========================================================================
    // Movimiento
    //=========================================================================

    void Drive(
        const Protocol::DriverCommand& command,
        const VehicleProfiles::DrivingProfile& profile) noexcept;
    //=====================================================================
    // Control
    //=====================================================================

    void Stop() noexcept;        

private:

    //=========================================================================
    // Hardware
    //=========================================================================

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