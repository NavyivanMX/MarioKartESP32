using System;
using System.IO;
using System.Threading.Tasks;

using Android.Bluetooth;

namespace MarioKart.Android.Communication.Bluetooth
{
    /// <summary>
    /// Transporte Bluetooth del protocolo MarioKart.
    /// Se encarga únicamente de enviar y recibir bytes.
    /// No conoce DriverCommand ni el protocolo.
    /// </summary>
    public sealed class BluetoothTransport : ICommunicationTransport
    {
        //---------------------------------------------------------------------
        // Campos
        //---------------------------------------------------------------------

        private readonly BluetoothManager manager;

        private Stream inputStream;

        private Stream outputStream;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public BluetoothTransport(BluetoothManager manager)
        {
            this.manager = manager;
        }

        //---------------------------------------------------------------------
        // Propiedades
        //---------------------------------------------------------------------

        public bool IsConnected =>
            manager.IsConnected;

        //---------------------------------------------------------------------
        // Conexión
        //---------------------------------------------------------------------

        public async Task<bool> ConnectAsync(
            BluetoothDevice device)
        {
            bool connected =
                await manager.ConnectAsync(device);

            if (!connected)
            {
                return false;
            }

            inputStream =
                manager.Socket.InputStream;

            outputStream =
                manager.Socket.OutputStream;

            return true;
        }

        public Task DisconnectAsync()
        {
            inputStream = null;

            outputStream = null;

            manager.Disconnect();

            return Task.CompletedTask;
        }

        //---------------------------------------------------------------------
        // Envío
        //---------------------------------------------------------------------

        public async Task SendAsync(
            byte[] data)
        {
            if (!IsConnected)
            {
                return;
            }

            if (data == null)
            {
                return;
            }

            if (data.Length == 0)
            {
                return;
            }

            await outputStream.WriteAsync(
                data,
                0,
                data.Length);

            await outputStream.FlushAsync();
        }

        //---------------------------------------------------------------------
        // Recepción
        //---------------------------------------------------------------------

        public async Task<int> ReceiveAsync(
            byte[] buffer)
        {
            if (!IsConnected)
            {
                return 0;
            }

            if (buffer == null)
            {
                return 0;
            }

            return await inputStream.ReadAsync(
                buffer,
                0,
                buffer.Length);
        }
    }
}