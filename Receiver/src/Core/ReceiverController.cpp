/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ReceiverController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del orquestador principal del receptor.
 ******************************************************************************/

#include "src/Core/ReceiverController.h"

#include "src/Debug/ConsoleLogger.h"

namespace MK
{

void ReceiverController::Initialize() noexcept
{
    ConsoleLogger::Instance().Initialize();

    ConsoleLogger::Instance().LogBoot();

    m_receiver.Initialize();

    m_vehicle.Initialize();
}

void ReceiverController::Update() noexcept
{
    if (m_receiver.HasNewCommand())
    {
        const auto& command = m_receiver.GetCommand();

        m_vehicle.Update(command);

        ConsoleLogger::Instance().LogCommand(command);
    }
}

}