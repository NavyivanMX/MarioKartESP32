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
    pinMode(Pins::Haptic, OUTPUT);
    digitalWrite(Pins::Haptic,LOW);
    return true;
}

//=============================================================================
// Actualización
//=============================================================================

void Controller::Update() noexcept
{
    //---------------------------------------------------------------------
    // Actualizar entradas
    //---------------------------------------------------------------------

    m_inputManager.Update();

    //---------------------------------------------------------------------
    // ¿Es momento de transmitir?
    //---------------------------------------------------------------------

    const std::uint32_t now =
        millis();

    if ((now - m_lastTransmitTime) <
        TransmitterConfig::TransmitPeriodMs)
    {
        return;
    }

    m_lastTransmitTime = now;

    //---------------------------------------------------------------------
    // Obtener comando actual
    //---------------------------------------------------------------------

    const auto& command =
        m_inputManager.GetDriverCommand();

        using Types::Vehicle::Turbo;


static bool lastTurbo = false;

bool turbo =
    command.turbo == Turbo::Enabled;

if (turbo != lastTurbo)
{
    Serial.printf(
        "Turbo -> %s\n",
        turbo ? "ON" : "OFF");

    lastTurbo = turbo;
}

digitalWrite(
    Pins::Haptic,
    turbo ? HIGH : LOW);      
    //---------------------------------------------------------------------
    // Mostrar solamente cuando cambie
    //---------------------------------------------------------------------

    if (command != m_lastLoggedCommand)
    {
        m_consoleLogger.Log(command);

        m_lastLoggedCommand =
            command;
    }

    //---------------------------------------------------------------------
    // Construir Packet
    //---------------------------------------------------------------------

    Protocol::Packet<
        Protocol::DriverCommand> packet;

    packet.header.type =
        Protocol::PacketType::DriverCommand;

    packet.header.payloadSize =
        sizeof(Protocol::DriverCommand);

    packet.payload =
        command;

    //---------------------------------------------------------------------
    // Serializar
    //---------------------------------------------------------------------

    std::uint8_t buffer[
        Protocol::PacketSize<
            Protocol::DriverCommand>()];

    if (!Protocol::PacketSerializer::Serialize(
            packet,
            buffer,
            sizeof(buffer)))
    {
        return;
    }

#if MK_DEBUG_PACKET_SERIALIZER

    Serial.println();
    Serial.println("========== PacketSerializer ==========");

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

#endif

    //---------------------------------------------------------------------
    // Enviar ESP-NOW
    //---------------------------------------------------------------------

    m_espNowHandler.Send(
        buffer,
        sizeof(buffer));
}

} // namespace MK