/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothManager.cs
 ******************************************************************************/

using System;
using System.Threading.Tasks;

using Android.Bluetooth;

namespace MarioKart.Android.Communication.Bluetooth
{
    /// <summary>
    /// Administra la conexión Bluetooth con el ESP32.
    /// No conoce el protocolo ni DriverCommand.
    /// Únicamente administra el socket Bluetooth.
    /// </summary>
    public sealed class BluetoothManager
    {
        //---------------------------------------------------------------------
        // Constantes
        //---------------------------------------------------------------------

        /// <summary>
        /// UUID estándar para Bluetooth Serial Port Profile (SPP).
        /// </summary>
        private static readonly Java.Util.UUID SerialPortUuid =
            Java.Util.UUID.FromString(
                "00001101-0000-1000-8000-00805F9B34FB");

        //---------------------------------------------------------------------
        // Campos
        //---------------------------------------------------------------------

        private readonly BluetoothAdapter adapter;

        private BluetoothSocket socket;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public BluetoothManager()
        {
            adapter = BluetoothAdapter.DefaultAdapter;
        }

        //---------------------------------------------------------------------
        // Propiedades
        //---------------------------------------------------------------------

        public bool IsBluetoothAvailable =>
            adapter != null;

        public bool IsBluetoothEnabled =>
            adapter?.IsEnabled ?? false;

        public bool IsConnected =>
            socket?.IsConnected ?? false;

        public BluetoothSocket Socket => socket;

        //---------------------------------------------------------------------
        // Conexión
        //---------------------------------------------------------------------

        public async Task<bool> ConnectAsync(BluetoothDevice device)
        {
            if (device == null)
                return false;

            try
            {
                Disconnect();

                socket =
                    device.CreateRfcommSocketToServiceRecord(
                        SerialPortUuid);

                adapter.CancelDiscovery();

                await socket.ConnectAsync();

                return socket.IsConnected;
            }
            catch (Exception)
            {
                Disconnect();

                return false;
            }
        }

        public void Disconnect()
        {
            try
            {
                socket?.Close();
            }
            catch
            {
            }

            socket?.Dispose();
            socket = null;
        }

        //---------------------------------------------------------------------
        // Streams
        //---------------------------------------------------------------------

        public System.IO.Stream GetInputStream()
        {
            return socket?.InputStream;
        }

        public System.IO.Stream GetOutputStream()
        {
            return socket?.OutputStream;
        }
    }
}