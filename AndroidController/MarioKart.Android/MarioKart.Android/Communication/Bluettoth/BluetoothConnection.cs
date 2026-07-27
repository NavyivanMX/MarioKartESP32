using System;

namespace MarioKart.Android.Communication.Bluetooth
{
    /// <summary>
    /// Representa el estado de una conexión Bluetooth.
    /// </summary>
    public sealed class BluetoothConnection
    {
        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public BluetoothConnection(
            BluetoothDeviceInfo device,
            bool isConnected)
        {
            Device = device;

            IsConnected = isConnected;

            ConnectedAt =
                isConnected
                    ? DateTime.Now
                    : (DateTime?)null;
        }

        //---------------------------------------------------------------------
        // Propiedades
        //---------------------------------------------------------------------

        /// <summary>
        /// Dispositivo remoto.
        /// </summary>
        public BluetoothDeviceInfo Device { get; }

        /// <summary>
        /// Indica si existe una conexión activa.
        /// </summary>
        public bool IsConnected { get; }

        /// <summary>
        /// Fecha y hora en que se estableció la conexión.
        /// </summary>
        public DateTime? ConnectedAt { get; }

        public BluetoothConnection Connection
        {
            get;
            private set;
        }

        //---------------------------------------------------------------------
        // Conversión
        //---------------------------------------------------------------------

        public override string ToString()
        {
            if (Device == null)
            {
                return "Disconnected";
            }

            return $"{Device.Name} ({Device.Address})";
        }
    }
}