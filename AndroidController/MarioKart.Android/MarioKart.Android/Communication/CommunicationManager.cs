using System;
using System.Threading.Tasks;

using Android.Bluetooth;

using MarioKart.Android.Communication.Bluetooth;
using BluetoothManager = MarioKart.Android.Communication.Bluetooth.BluetoothManager;

namespace MarioKart.Android.Communication
{
    /// <summary>
    /// Administra el transporte de comunicación activo.
    /// </summary>
    public sealed class CommunicationManager
    {
        //---------------------------------------------------------------------
        // Campos
        //---------------------------------------------------------------------

        private readonly BluetoothManager bluetoothManager;

        private readonly BluetoothTransport bluetoothTransport;

        private ICommunicationTransport activeTransport;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public CommunicationManager(            BluetoothTransport bluetoothTransport)
        {
            this.bluetoothTransport = bluetoothTransport;
            activeTransport = bluetoothTransport;
        }

        //---------------------------------------------------------------------
        // Estado
        //---------------------------------------------------------------------

        public bool IsConnected =>  activeTransport?.IsConnected ?? false;

        //---------------------------------------------------------------------
        // Acceso
        //---------------------------------------------------------------------

        public ICommunicationTransport Transport =>            activeTransport;

        //---------------------------------------------------------------------
        // Bluetooth
        //---------------------------------------------------------------------

        public async Task<bool> ConnectBluetoothAsync(
            BluetoothDevice device)
        {
            return await bluetoothTransport
                .ConnectAsync(device);
        }

        //---------------------------------------------------------------------
        // Desconexión
        //---------------------------------------------------------------------

        public async Task DisconnectAsync()
        {
            if (activeTransport == null)
            {
                return;
            }

            await activeTransport
                .DisconnectAsync();
        }

        //---------------------------------------------------------------------
        // Comunicación
        //---------------------------------------------------------------------

        public async Task SendAsync(
            byte[] data)
        {
            if (!IsConnected)
            {
                return;
            }

            await activeTransport
                .SendAsync(data);
        }

        public async Task<int> ReceiveAsync(
            byte[] buffer)
        {
            if (!IsConnected)
            {
                return 0;
            }

            return await activeTransport
                .ReceiveAsync(buffer);
        }
    }
}