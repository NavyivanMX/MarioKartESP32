/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothManager.cpp
 ******************************************************************************/

#include "BluetoothManager.h"

#include "Protocol/VehicleStatusSerializer.h"

namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

bool BluetoothManager::Begin(
    const char* deviceName) noexcept
{
    return m_transport.Begin(deviceName);
}

//=============================================================================

bool BluetoothManager::Connected() const noexcept
{
    return m_transport.Connected();
}

//=============================================================================
// Recepción
//=============================================================================

bool BluetoothManager::Receive(
    Protocol::DriverCommand& command) noexcept
{
    (void)command;

    //---------------------------------------------------------------------
    // Pendiente:
    // Leer PacketHeader
    // Validar PacketType
    // Deserializar DriverCommand
    //---------------------------------------------------------------------

    return false;
}

//=============================================================================
// Envío
//=============================================================================

bool BluetoothManager::Send(
    const Protocol::VehicleStatus& status) noexcept
{
    std::uint8_t payload[
        sizeof(Protocol::VehicleStatus)];

    if (!Protocol::SerializeVehicleStatus(
            status,
            payload,
            sizeof(payload)))
    {
        return false;
    }

    //---------------------------------------------------------------------
    // De momento enviamos únicamente el payload.
    //
    // En la siguiente etapa este payload será encapsulado dentro
    // de un Packet<VehicleStatus>.
    //---------------------------------------------------------------------

    return m_transport.Send(
        payload,
        sizeof(payload));
}

}