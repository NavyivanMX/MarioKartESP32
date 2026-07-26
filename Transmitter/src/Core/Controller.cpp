/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Controller.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del orquestador principal del transmisor.
 ******************************************************************************/

#include "Controller.h"
#include "src/Config/TransmitterConfig.h"
#include <MKShared.h>

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

    return m_espNowHandler.Begin();
}

//=============================================================================
// Actualización
//=============================================================================

void Controller::Update() noexcept
{
    //---------------------------------------------------------------------
    // Leer entradas
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
    // Mostrar en consola
    //---------------------------------------------------------------------

    m_consoleLogger.Log(command);

    //---------------------------------------------------------------------
    // Serializar paquete
    //---------------------------------------------------------------------

    std::uint8_t packet[
        Protocol::DriverCommandSerializer::PacketSize];

    if (!Protocol::DriverCommandSerializer::Serialize(
            command,
            packet))
    {
        return;
    }

    //---------------------------------------------------------------------
    // Enviar mediante ESP-NOW
    //---------------------------------------------------------------------

    m_espNowHandler.Send(
        packet,
        sizeof(packet));
}

} // namespace MK