/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : VehicleController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del orquestador principal del vehículo.
 ******************************************************************************/

#include "VehicleController.h"

namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

bool VehicleController::Begin() noexcept
{
    if (!m_motion.Begin())
    {
        return false;
    }

    if (!m_gravity.Begin())
    {
        return false;
    }

    return true;
}

//=============================================================================
// Control
//=============================================================================

void VehicleController::Update(
    const Protocol::DriverCommand& command,
    const VehicleProfiles::DrivingProfile& profile) noexcept
{
    
    //---------------------------------------------------------------------
    // Movimiento
    //---------------------------------------------------------------------    
    m_motion.Update(
        command,
        profile);

    //---------------------------------------------------------------------
    // Gravity
    //---------------------------------------------------------------------

    m_gravity.Update(
        command.driveMode);
}

} // namespace MK