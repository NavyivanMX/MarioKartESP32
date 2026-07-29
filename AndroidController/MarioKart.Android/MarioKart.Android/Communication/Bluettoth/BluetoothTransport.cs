/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothTransport.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación de ICommunicationTransport utilizando Bluetooth Classic.
 ******************************************************************************/

using System.IO;
using System.Threading.Tasks;

using Android.Bluetooth;

using MarioKart.Android.Debug;

namespace MarioKart.Android.Communication.Bluetooth
{
    public sealed class BluetoothTransport
        : ICommunicationTransport
    {
        //---------------------------------------------------------------------
        // Campos
        //---------------------------------------------------------------------

        private readonly BluetoothManager m_bluetoothManager;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public BluetoothTransport(
            BluetoothManager bluetoothManager)
        {
            m_bluetoothManager = bluetoothManager;
        }

        //---------------------------------------------------------------------
        // Propiedades
        //---------------------------------------------------------------------

        public bool IsConnected =>
            m_bluetoothManager.IsConnected;

        //---------------------------------------------------------------------
        // Conexión
        //---------------------------------------------------------------------

        public async Task<bool> ConnectAsync(
            BluetoothDevice device)
        {
            return await m_bluetoothManager
                .ConnectAsync(device);
        }

        public Task DisconnectAsync()
        {
            m_bluetoothManager.Disconnect();

            return Task.CompletedTask;
        }

        //---------------------------------------------------------------------
        // Comunicación
        //---------------------------------------------------------------------

        public async Task SendAsync(
            byte[] packet)
        {
            if (packet == null)
            {
                return;
            }

            if (!IsConnected)
            {
                ConsoleLogger.Warning(
                    "BluetoothTransport: no active connection.");

                return;
            }

            Stream stream =
                m_bluetoothManager.GetOutputStream();

            if (stream == null)
            {
                return;
            }

            try
            {
                await stream.WriteAsync(
                    packet,
                    0,
                    packet.Length);
            }
            catch
            {
                await DisconnectAsync();
            }
        }

        public async Task<int> ReceiveAsync(
            byte[] buffer)
        {
            if (!IsConnected)
            {
                return 0;
            }

            Stream stream =
                m_bluetoothManager.GetInputStream();

            if (stream == null)
            {
                return 0;
            }

            try
            {
                return await stream.ReadAsync(
                    buffer,
                    0,
                    buffer.Length);
            }
            catch
            {
                await DisconnectAsync();

                return 0;
            }
        }
    }
}