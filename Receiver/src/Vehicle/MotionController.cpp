/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotionController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del controlador de movimiento del vehículo.
 ******************************************************************************/

#include "src/Vehicle/MotionController.h"

namespace MK
{

bool MotionController::Begin() noexcept
{
    return m_motor.Begin();
}

void MotionController::Stop() noexcept
{
    m_motor.Stop();
}

void MotionController::Update(
    const Protocol::DriverCommand& command,
    const VehicleProfiles::DrivingProfile& profile) noexcept
{
    m_motor.Drive(
        command,
        profile);
}

}