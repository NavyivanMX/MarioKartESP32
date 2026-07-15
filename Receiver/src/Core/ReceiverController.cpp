/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ReceiverController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del orquestador principal del firmware Receiver.
 ******************************************************************************/

#include "src/Core/ReceiverController.h"

namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

bool ReceiverController::Begin() noexcept
{
    m_logger.Begin();

    m_logger.LogBoot();

    if (!m_receiver.Begin())
    {
        m_logger.LogError(
            "ESPNowReceiver initialization failed.");

        return false;
    }

    if (!m_vehicle.Begin())
    {
        m_logger.LogError(
            "VehicleController initialization failed.");

        return false;
    }

    m_logger.LogReady();

    return true;
}

//=============================================================================
// Ciclo principal
//=============================================================================

void ReceiverController::Update() noexcept
{
    
    if (!m_receiver.HasNewCommand())
    {
        return;
    }

    const auto& command = m_receiver.GetCommand();

    m_vehicle.Update(command);

    m_logger.Log(command);
}

} // namespace MK