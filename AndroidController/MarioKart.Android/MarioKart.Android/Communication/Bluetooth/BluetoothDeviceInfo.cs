/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothDeviceInfo.cs
 ******************************************************************************/

using Android.Bluetooth;

namespace MarioKart.Android.Communication.Bluetooth
{
    public sealed class BluetoothDeviceInfo
    {
        public string Name { get; set; }

        public string Address { get; set; }

        public BluetoothDevice Device { get; set; }

        public override string ToString()
        {
            return $"{Name} ({Address})";
        }
    }
}