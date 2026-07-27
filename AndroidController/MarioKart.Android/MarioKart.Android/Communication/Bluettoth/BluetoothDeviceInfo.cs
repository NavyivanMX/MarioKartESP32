using Android.Bluetooth;

namespace MarioKart.Android.Communication.Bluetooth
{
    /// <summary>
    /// Representa un dispositivo Bluetooth descubierto por la aplicación.
    /// Encapsula la información necesaria para establecer una conexión.
    /// </summary>
    public sealed class BluetoothDeviceInfo
    {
        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public BluetoothDeviceInfo(
            string name,
            string address,
            BluetoothDevice device)
        {
            Name = name ?? string.Empty;

            Address = address ?? string.Empty;

            Device = device;
        }

        //---------------------------------------------------------------------
        // Propiedades
        //---------------------------------------------------------------------

        /// <summary>
        /// Nombre visible del dispositivo.
        /// </summary>
        public string Name { get; }

        /// <summary>
        /// Dirección MAC del dispositivo.
        /// </summary>
        public string Address { get; }

        /// <summary>
        /// Dispositivo Bluetooth de Android.
        /// </summary>
        public BluetoothDevice Device { get; }

        //---------------------------------------------------------------------
        // Conversión
        //---------------------------------------------------------------------

        public override string ToString()
        {
            return $"{Name} ({Address})";
        }
    }
}