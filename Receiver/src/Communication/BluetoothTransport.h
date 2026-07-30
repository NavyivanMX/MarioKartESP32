/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothTransport.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Transporte Bluetooth Classic para la recepción y transmisión
 * de DriverCommand.
 ******************************************************************************/

#ifndef MK_RECEIVER_BLUETOOTH_TRANSPORT_H
#define MK_RECEIVER_BLUETOOTH_TRANSPORT_H

//=============================================================================
// Includes
//=============================================================================

#include <BluetoothSerial.h>

#include <MKShared.h>

namespace MK
{

class BluetoothTransport final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    [[nodiscard]]
    bool Initialize(
        const char* deviceName);

    //=========================================================================
    // Estado
    //=========================================================================

    [[nodiscard]]
    bool IsConnected();

    //=========================================================================
    // Comunicación
    //=========================================================================

    [[nodiscard]]
    bool Receive(
        Protocol::DriverCommand& command);

    [[nodiscard]]
    bool Send(
        const Protocol::DriverCommand& command);

private:

    //=========================================================================
    // Bluetooth Classic
    //=========================================================================

    BluetoothSerial m_serial;
};

} // namespace MK

#endif // MK_RECEIVER_BLUETOOTH_TRANSPORT_H