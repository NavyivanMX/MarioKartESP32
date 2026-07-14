/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotorController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del controlador de motores.
 ******************************************************************************/

#include "src/Vehicle/MotorController.h"

#include "src/Debug/ConsoleLogger.h"

namespace MK
{

void MotorController::Initialize() noexcept
{
}

void MotorController::Update(
    const MotorState& left,
    const MotorState& right) noexcept
{
    ConsoleLogger::Instance().LogMotorState(
        left,
        right);

    //
    // Próximamente:
    //
    // m_leftMotor.Update(left);
    // m_rightMotor.Update(right);
    //
}

}