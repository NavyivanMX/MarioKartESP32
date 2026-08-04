/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothManager.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del administrador del protocolo Bluetooth.
 ******************************************************************************/

#include "BluetoothManager.h"

namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

bool BluetoothManager::Begin(
    const char* deviceName) noexcept
{
    Serial.printf("BTM Begin.");
    return m_transport.Begin(
        deviceName);
}

//=============================================================================
// Estado
//=============================================================================

bool BluetoothManager::Connected() const noexcept
{
    Serial.printf("BTM Connected.");
    return m_transport.Connected();
}

//=============================================================================
// DriverCommand
//=============================================================================

bool BluetoothManager::Receive(
    Protocol::DriverCommand& command) noexcept
{
    Serial.println("1");

    // std::uint8_t buffer[
    //     Protocol::PacketSize<
    //         Protocol::DriverCommand>()];

    std::uint8_t buffer[7];            

    Serial.println("2");

    // const std::size_t received =
    //     m_transport.Receive(
    //         buffer,
    //         sizeof(buffer));

    const std::size_t received =
    m_transport.Receive(
        buffer,
        7);

    Serial.printf(
        "Received=%u Expected=%u\n",
        received,
        sizeof(buffer));

    //Serial.printf("RX Type=%u  Expected=%u\n",(uint8_t)packet.header.type,(uint8_t)Protocol::PacketType::DriverCommand);

    Serial.printf("sizeof(PacketHeader)=%u\n", sizeof(Protocol::PacketHeader));
    Serial.printf("sizeof(DriverCommand)=%u\n", sizeof(Protocol::DriverCommand));
    Serial.printf("PacketSize=%u\n", Protocol::PacketSize<Protocol::DriverCommand>());

    if(received > 0)
    {
        Serial.print("RX = ");

        for(std::size_t i=0;i<received;i++)
        {
            Serial.printf("%02X ",buffer[i]);
        }

        Serial.println();
    }

    if(received != sizeof(buffer))
    {
        Serial.println("3");

        return false;
    }

    Serial.println("4");

    Protocol::Packet<
        Protocol::DriverCommand> packet;

    if(!Protocol::PacketSerializer::Deserialize(
            buffer,
            received,
            packet))
    {
        Serial.println("5");

        return false;
    }

    Serial.println("6");

    if(packet.header.type !=
       Protocol::PacketType::DriverCommand)
    {
        Serial.printf(
            "Wrong Type=%u\n",
            (uint8_t)packet.header.type);

        return false;
    }

    Serial.println("7");

    command =
        packet.payload;

    return true;
}

//=============================================================================
// VehicleStatus
//=============================================================================

bool BluetoothManager::Send(
    const Protocol::VehicleStatus& status) noexcept
{
    Serial.printf("BTM Send Begin.");
    //-------------------------------------------------------------
    // Construir Packet
    //-------------------------------------------------------------

    Protocol::Packet<
        Protocol::VehicleStatus> packet;

    packet.header.type =
        Protocol::PacketType::VehicleStatus;

    packet.header.payloadSize =
        sizeof(Protocol::VehicleStatus);

    packet.payload = status;

    //-------------------------------------------------------------
    // Serializar
    //-------------------------------------------------------------

    std::uint8_t buffer[
        Protocol::PacketSize<
            Protocol::VehicleStatus>()];

    if (!Protocol::PacketSerializer::Serialize(
            packet,
            buffer,
            sizeof(buffer)))
    {
        return false;
    }

    //-------------------------------------------------------------
    // Enviar
    //-------------------------------------------------------------
    Serial.printf("BTM Send End.");
    return m_transport.Send(
        buffer,
        sizeof(buffer));
}

}