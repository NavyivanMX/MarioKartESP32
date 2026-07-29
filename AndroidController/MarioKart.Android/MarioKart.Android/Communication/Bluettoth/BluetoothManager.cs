/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothManager.cs
 *
 * Descripción:
 * Administra la conexión Bluetooth Classic (RFCOMM) con el ESP32.
 ******************************************************************************/

using System;
using System.IO;
using System.Threading.Tasks;

using Android.Bluetooth;

using MarioKart.Android.Debug;

namespace MarioKart.Android.Communication.Bluetooth
{
    public sealed class BluetoothManager
    {
        //---------------------------------------------------------------------
        // Constantes
        //---------------------------------------------------------------------

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
            adapter =
                BluetoothAdapter.DefaultAdapter;
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

        //---------------------------------------------------------------------
        // Conexión
        //---------------------------------------------------------------------

        public async Task<bool> ConnectAsync(
            BluetoothDevice device)
        {
            if (device == null)
            {
                return false;
            }

            try
            {
                Disconnect();

                ConsoleLogger.Log(
                    $"Connecting to {device.Name}...");

                socket =
                    device.CreateRfcommSocketToServiceRecord(
                        SerialPortUuid);

                adapter.CancelDiscovery();

                await socket.ConnectAsync();

                ConsoleLogger.Log(
                    $"Connected to {device.Name}");

                return socket.IsConnected;
            }
            catch (Exception ex)
            {
                ConsoleLogger.Exception(ex);

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

            ConsoleLogger.Log(
                "Bluetooth disconnected.");
        }

        //---------------------------------------------------------------------
        // Comunicación
        //---------------------------------------------------------------------

        public bool Send(byte[] packet)
        {
            if (!IsConnected)
            {
                ConsoleLogger.Warning(
                    "Bluetooth not connected.");

                return false;
            }

            if (packet == null)
            {
                return false;
            }

            try
            {
                Stream stream =
                    socket.OutputStream;

                stream.Write(
                    packet,
                    0,
                    packet.Length);

                stream.Flush();

                ConsoleLogger.Log(
                    $"Bluetooth TX ({packet.Length} bytes)");

                return true;
            }
            catch (Exception ex)
            {
                ConsoleLogger.Exception(ex);

                Disconnect();

                return false;
            }
        }

        //---------------------------------------------------------------------
        // Streams
        //---------------------------------------------------------------------

        public Stream GetInputStream()
        {
            return socket?.InputStream;
        }

        public Stream GetOutputStream()
        {
            return socket?.OutputStream;
        }
    }
}