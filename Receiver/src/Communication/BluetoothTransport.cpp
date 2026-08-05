/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothTransport.cpp
 ******************************************************************************/

#include "BluetoothTransport.h"

#include <BluetoothSerial.h>

namespace MK
{

namespace
{
BluetoothSerial g_serial;
}

//=============================================================================

bool BluetoothTransport::Begin(
    const char* deviceName) noexcept
{
    return g_serial.begin(deviceName);
}

//=============================================================================

// bool BluetoothTransport::Connected() const noexcept
// {
//     return g_serial.hasClient();
// }

bool BluetoothTransport::Connected() const noexcept
{
    bool connected =
        g_serial.hasClient();
    return connected;
}
//=============================================================================

std::size_t BluetoothTransport::Receive(
    std::uint8_t* buffer,
    std::size_t length) noexcept
{
    if (!Connected())
    {
        return 0;
    }

    return g_serial.readBytes(
        reinterpret_cast<char*>(buffer),
        length);
}

//=============================================================================

bool BluetoothTransport::Send(
    const std::uint8_t* data,
    std::size_t length) noexcept
{
    if (!Connected())
    {
        return false;
    }

    return g_serial.write(
               data,
               length) == length;
}

} // namespace MK