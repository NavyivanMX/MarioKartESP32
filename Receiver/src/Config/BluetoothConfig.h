/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothConfig.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Configuración centralizada del Bluetooth Classic.
 ******************************************************************************/

#ifndef MK_RECEIVER_BLUETOOTH_CONFIG_H
#define MK_RECEIVER_BLUETOOTH_CONFIG_H

namespace MK::Config
{

class BluetoothConfig final
{
public:

    //---------------------------------------------------------------------
    // Nombre del dispositivo Bluetooth
    //---------------------------------------------------------------------

    static constexpr const char* DeviceName =
        "MarioKart RC";

    //---------------------------------------------------------------------
    // PIN (opcional)
    //---------------------------------------------------------------------

    static constexpr const char* Pin =
        "1234";
};

} // namespace MK::Config

#endif // MK_RECEIVER_BLUETOOTH_CONFIG_H