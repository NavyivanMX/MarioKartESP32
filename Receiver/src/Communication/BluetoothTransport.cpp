/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothTransport.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del transporte Bluetooth Classic.
 ******************************************************************************/

#include "BluetoothTransport.h"

namespace MK
{

//=============================================================================
// Inicialización
//=============================================================================

bool BluetoothTransport::Initialize(
    const char* deviceName)
{
    if (deviceName == nullptr)
    {
        return false;
    }

    return m_serial.begin(deviceName);
}

//=============================================================================
// Estado
//=============================================================================

bool BluetoothTransport::IsConnected()
{
    return m_serial.hasClient();
}

//=============================================================================
// Recepción
//=============================================================================

bool BluetoothTransport::Receive(
    Protocol::DriverCommand& command)
{
    //-------------------------------------------------------------
    // Cliente conectado
    //-------------------------------------------------------------

    if (!IsConnected())
    {
        return false;
    }

    //-------------------------------------------------------------
    // Debe existir un paquete completo
    //-------------------------------------------------------------

    constexpr std::size_t PacketSize =
        Protocol::DriverCommandSerializer::PacketSize;

    if (m_serial.available() < PacketSize)
    {
        return false;
    }

    //-------------------------------------------------------------
    // Leer paquete
    //-------------------------------------------------------------

    std::uint8_t packet[PacketSize];

    std::size_t bytesRead =
        m_serial.readBytes(
            reinterpret_cast<char*>(packet),
            PacketSize);

    if (bytesRead != PacketSize)
    {
        return false;
    }

    //-------------------------------------------------------------
    // Deserializar
    //-------------------------------------------------------------

    return Protocol::DriverCommandSerializer::
        Deserialize(
            packet,
            command);
}

//=============================================================================
// Envío
//=============================================================================

bool BluetoothTransport::Send(
    const Protocol::DriverCommand& command)
{
    if (!IsConnected())
    {
        return false;
    }

    std::uint8_t packet[
        Protocol::DriverCommandSerializer::PacketSize];

    if (!Protocol::DriverCommandSerializer::
            Serialize(
                command,
                packet))
    {
        return false;
    }

    return m_serial.write(
               packet,
               sizeof(packet))
           == sizeof(packet);
}

} // namespace MK