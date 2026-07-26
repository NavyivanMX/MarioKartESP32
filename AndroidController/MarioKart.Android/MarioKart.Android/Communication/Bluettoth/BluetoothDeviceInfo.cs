/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothDeviceInfo.cs
 *
 * Descripción:
 * Representa un dispositivo Bluetooth descubierto por la aplicación.
 ******************************************************************************/

namespace MarioKart.Android.Communication.Bluetooth
{

    public sealed class BluetoothDeviceInfo
    {
        public string Name { get; set; }

        public string Address { get; set; }

        public override string ToString()
        {
            return $"{Name} ({Address})";
        }
    }

}