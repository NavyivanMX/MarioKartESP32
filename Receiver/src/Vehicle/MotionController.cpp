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

void MotionController::Update(
    const Protocol::DriverCommand& command,
    const VehicleProfiles::DrivingProfile& profile) noexcept
{
    Serial.printf(
    "Direction=%u\n",
    (uint8_t)command.direction);

Serial.printf(
    "Turbo=%u\n",
    (uint8_t)command.turbo);

Serial.printf(
    "DriveMode=%u\n",
    (uint8_t)command.driveMode);
    
    m_motor.Drive(
        command,
        profile);
}

}