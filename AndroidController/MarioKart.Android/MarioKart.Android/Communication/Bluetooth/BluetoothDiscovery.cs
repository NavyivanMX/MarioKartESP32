/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothDiscovery.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Encapsula la búsqueda de dispositivos Bluetooth emparejados.
 ******************************************************************************/

using System.Collections.Generic;
using System.Linq;

using Android.Bluetooth;

namespace MarioKart.Android.Communication.Bluetooth
{
    public sealed class BluetoothDiscovery
    {
        //---------------------------------------------------------------------
        // Campos
        //---------------------------------------------------------------------

        private readonly BluetoothAdapter m_adapter;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public BluetoothDiscovery()
        {
            m_adapter =
                BluetoothAdapter.DefaultAdapter;
        }

        //---------------------------------------------------------------------
        // Estado
        //---------------------------------------------------------------------

        public bool IsSupported =>
            m_adapter != null;

        public bool IsEnabled =>
            m_adapter?.IsEnabled ?? false;

        //---------------------------------------------------------------------
        // Dispositivos emparejados
        //---------------------------------------------------------------------

        public IReadOnlyList<BluetoothDeviceInfo>
            GetPairedDevices()
        {
            if (!IsSupported ||
                !IsEnabled)
            {
                return new List<BluetoothDeviceInfo>();
            }

            return m_adapter
                .BondedDevices
                .Select(device =>
                    new BluetoothDeviceInfo
                    {
                        Name = device.Name,
                        Address = device.Address,
                        Device = device
                    })
                .OrderBy(device => device.Name)
                .ToList();
        }
    }
}