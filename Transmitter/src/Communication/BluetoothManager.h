/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothManager.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Encapsula la comunicación Bluetooth Classic del transmisor.
 * Permite recibir DriverCommand enviados desde la aplicación Android.
 ******************************************************************************/

#ifndef MK_BLUETOOTH_MANAGER_H
#define MK_BLUETOOTH_MANAGER_H

#include <BluetoothSerial.h>

#include <MKShared.h>

namespace MK
{

class BluetoothManager final
{
public:

    BluetoothManager() = default;

    ~BluetoothManager() = default;

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    [[nodiscard]]
    bool Begin() noexcept;

    void End() noexcept;

    //=========================================================================
    // Comunicación
    //=========================================================================

    [[nodiscard]]
    bool IsConnected() const noexcept;

    [[nodiscard]]
    bool Receive(
        Protocol::DriverCommand& command) noexcept;

private:

    BluetoothSerial m_serial;
};

} // namespace MK

#endif // MK_BLUETOOTH_MANAGER_H