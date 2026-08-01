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
    return m_transport.Begin(
        deviceName);
}

//=============================================================================
// Estado
//=============================================================================

bool BluetoothManager::Connected() const noexcept
{
    return m_transport.Connected();
}

//=============================================================================
// DriverCommand
//=============================================================================

bool BluetoothManager::Receive(
    Protocol::DriverCommand& command) noexcept
{
    std::uint8_t buffer[
        Protocol::PacketSize<
            Protocol::DriverCommand>()];

    const std::size_t received =
        m_transport.Receive(
            buffer,
            sizeof(buffer));

    if (received != sizeof(buffer))
    {
        return false;
    }

    Protocol::Packet<
        Protocol::DriverCommand> packet;

    if (!Protocol::PacketSerializer::Deserialize(
            buffer,
            received,
            packet))
    {
        return false;
    }

    if (packet.header.type !=
        Protocol::PacketType::DriverCommand)
    {
        return false;
    }

    command = packet.payload;

    return true;
}

//=============================================================================
// VehicleStatus
//=============================================================================

bool BluetoothManager::Send(
    const Protocol::VehicleStatus& status) noexcept
{
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

    return m_transport.Send(
        buffer,
        sizeof(buffer));
}

}