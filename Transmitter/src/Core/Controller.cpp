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

    return true;
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
    // Construir Packet
    //---------------------------------------------------------------------

    Protocol::Packet<
        Protocol::DriverCommand> packet;

    packet.header.type =
        Protocol::PacketType::DriverCommand;

    packet.header.payloadSize =
        sizeof(Protocol::DriverCommand);

    packet.payload = command;

    //---------------------------------------------------------------------
    // Serializar
    //---------------------------------------------------------------------

    std::uint8_t buffer[
        Protocol::PacketSize<
            Protocol::DriverCommand>()];

    /*
    //=============================================================
    // CÓDIGO ACTUAL
    //=============================================================

    if (!Protocol::PacketSerializer::Serialize(
            packet,
            buffer,
            sizeof(buffer)))
    {
        return;
    }
    */

    //=============================================================
    // PRUEBA DE DIAGNÓSTICO
    //=============================================================

    const bool serialized =
        Protocol::PacketSerializer::Serialize(
            packet,
            buffer,
            sizeof(buffer));

    Serial.println();
    Serial.println("========== PacketSerializer ==========");

    Serial.print("Serialize() : ");
    Serial.println(serialized ? "SUCCESS" : "FAILED");

    if (!serialized)
    {
        Serial.println("PacketSerializer returned FALSE.");
        Serial.println("Transmission cancelled.");
        Serial.println("======================================");
        return;
    }
    
    for (std::size_t i = 0; i < sizeof(buffer); ++i)
    {
        if (buffer[i] < 16)
        {
            Serial.print('0');
        }

        Serial.print(buffer[i], HEX);
        Serial.print(' ');
    }

    Serial.println();
    Serial.println("======================================");

    //---------------------------------------------------------------------
    // Enviar mediante ESP-NOW
    //---------------------------------------------------------------------

    m_espNowHandler.Send(
        buffer,
        sizeof(buffer));

        const bool sent =
    m_espNowHandler.Send(
        buffer,
        sizeof(buffer));

Serial.print("ESPNow Send(): ");
Serial.println(sent ? "SUCCESS" : "FAILED");
}

} // namespace MK