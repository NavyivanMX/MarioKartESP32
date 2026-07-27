using System.Collections.Generic;

using Android.Bluetooth;

namespace MarioKart.Android.Communication.Bluetooth
{
    /// <summary>
    /// Descubre los dispositivos Bluetooth disponibles.
    /// Actualmente utiliza los dispositivos previamente emparejados
    /// con Android.
    /// </summary>
    public sealed class BluetoothDiscovery
    {
        //---------------------------------------------------------------------
        // Campos
        //---------------------------------------------------------------------

        private readonly BluetoothManager manager;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public BluetoothDiscovery(
            BluetoothManager manager)
        {
            this.manager = manager;
        }

        //---------------------------------------------------------------------
        // Descubrimiento
        //---------------------------------------------------------------------

        /// <summary>
        /// Obtiene la lista de dispositivos Bluetooth emparejados.
        /// </summary>
        public IReadOnlyList<BluetoothDeviceInfo> GetPairedDevices()
        {
            var devices =
                new List<BluetoothDeviceInfo>();

            if (!manager.IsBluetoothAvailable)
            {
                return devices;
            }

            if (!manager.IsBluetoothEnabled)
            {
                return devices;
            }

            var adapter =
                BluetoothAdapter.DefaultAdapter;

            if (adapter == null)
            {
                return devices;
            }

            foreach (BluetoothDevice device in adapter.BondedDevices)
            {
                devices.Add(
                    new BluetoothDeviceInfo(
                        device.Name,
                        device.Address,
                        device));
            }

            return devices;
        }
    }
}