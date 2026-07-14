/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : VehicleController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 ******************************************************************************/

#include "src/Vehicle/VehicleController.h"

namespace MK
{

void VehicleController::Initialize() noexcept
{
    m_motion.Initialize();

    m_gravity.Initialize();
}

void VehicleController::Update(
    const Protocol::DriverCommand& command) noexcept
{
    if (command.driveMode == Types::Vehicle::DriveMode::Gravity)
    {
        m_gravity.Enable();
    }
    else
    {
        m_gravity.Disable();
    }

    m_motion.Update(command);
}

}