/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Controller.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del orquestador principal.
 ******************************************************************************/

#include "Controller.h"

#include "src/Config/TransmitterConfig.h"

namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

bool Controller::Begin()
{
    m_consoleLogger.Begin();

    m_consoleLogger.LogBoot();

    m_inputManager.Begin();

    if constexpr (!TransmitterConfig::InputTestMode)
    {
        if (!m_espNowHandler.Begin())
        {
            m_consoleLogger.LogError(
                "ESP-NOW initialization failed.");

            return false;
        }
    }

    m_consoleLogger.LogReady();

    return true;
}

//=============================================================================
// Ciclo principal
//=============================================================================

void Controller::Update() noexcept
{
    //---------------------------------------------------------------------
    // Actualizar entradas
    //---------------------------------------------------------------------

    if (!m_inputManager.Update())
    {
        return;
    }

    //---------------------------------------------------------------------
    // Obtener comando
    //---------------------------------------------------------------------

    const auto& command =
        m_inputManager.GetDriverCommand();

    //---------------------------------------------------------------------
    // Mostrar comando
    //---------------------------------------------------------------------

    m_consoleLogger.Log(command);

    //---------------------------------------------------------------------
    // Modo prueba
    //---------------------------------------------------------------------

    if constexpr (TransmitterConfig::InputTestMode)
    {
        return;
    }

    //---------------------------------------------------------------------
    // Comunicación
    //---------------------------------------------------------------------

    if (!m_espNowHandler.Send(command))
    {
        m_consoleLogger.LogError(
            "Packet not sent");
    }
}

} // namespace MK