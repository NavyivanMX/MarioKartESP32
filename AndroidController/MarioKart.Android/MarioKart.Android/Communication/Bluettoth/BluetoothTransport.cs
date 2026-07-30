using System;
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

        private readonly BluetoothManager m_bluetoothManager;

        //---------------------------------------------------------------------

        public BluetoothTransport(
            BluetoothManager bluetoothManager)
        {
            if (bluetoothManager == null)
            {
                throw new ArgumentNullException(
                    nameof(bluetoothManager));
            }

            m_bluetoothManager =
                bluetoothManager;
        }

        //---------------------------------------------------------------------

        public bool IsConnected =>
            m_bluetoothManager.IsConnected;

        //---------------------------------------------------------------------

        public Task<bool> ConnectAsync(
            BluetoothDevice device)
        {
            return m_bluetoothManager
                .ConnectAsync(device);
        }

        //---------------------------------------------------------------------

        public Task DisconnectAsync()
        {
            m_bluetoothManager.Disconnect();

            return Task.CompletedTask;
        }

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

                await stream.FlushAsync();

                ConsoleLogger.Log(
                    $"TX [{packet.Length}] : {BitConverter.ToString(packet)}");
            }
            catch (Exception ex)
            {
                ConsoleLogger.Exception(ex);

                await DisconnectAsync();
            }
        }

        //---------------------------------------------------------------------

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
                int count =
                    await stream.ReadAsync(
                        buffer,
                        0,
                        buffer.Length);

                if (count > 0)
                {
                    ConsoleLogger.Log(
                        $"RX [{count}] : {BitConverter.ToString(buffer, 0, count)}");
                }

                return count;
            }
            catch (Exception ex)
            {
                ConsoleLogger.Exception(ex);

                await DisconnectAsync();

                return 0;
            }
        }
    }
}