/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothManager.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Gestiona el protocolo de comunicación Bluetooth.
 ******************************************************************************/

#ifndef MK_RECEIVER_BLUETOOTHMANAGER_H
#define MK_RECEIVER_BLUETOOTHMANAGER_H

//=============================================================================
// Includes
//=============================================================================

#include <MKShared.h>

#include "BluetoothTransport.h"
#include "Protocol/VehicleStatus.h"

namespace MK
{

class BluetoothManager final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    [[nodiscard]]
    bool Begin(
        const char* deviceName) noexcept;

    //=========================================================================
    // Estado
    //=========================================================================

    [[nodiscard]]
    bool Connected() const noexcept;

    //=========================================================================
    // DriverCommand
    //=========================================================================

    [[nodiscard]]
    bool Receive(
        Protocol::DriverCommand& command) noexcept;

    //=========================================================================
    // VehicleStatus
    //=========================================================================

    [[nodiscard]]
    bool Send(
        const Protocol::VehicleStatus& status) noexcept;

private:

    BluetoothTransport m_transport;
};

}

#endif