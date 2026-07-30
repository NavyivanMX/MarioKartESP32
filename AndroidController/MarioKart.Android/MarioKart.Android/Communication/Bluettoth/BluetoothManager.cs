/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothManager.cs
 * Autor    : Narciso Ivan Cisneros Acosta
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

        private readonly BluetoothAdapter m_adapter;

        private readonly object m_syncRoot =
            new object();

        private BluetoothSocket m_socket;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public BluetoothManager()
        {
            m_adapter =
                BluetoothAdapter.DefaultAdapter;
        }

        //---------------------------------------------------------------------
        // Propiedades
        //---------------------------------------------------------------------

        public bool IsBluetoothAvailable =>
            m_adapter != null;

        public bool IsBluetoothEnabled =>
            m_adapter?.IsEnabled ?? false;

        public bool IsConnected =>
            m_socket?.IsConnected ?? false;

        /// <summary>
        /// Expone el socket únicamente para inspección.
        /// </summary>
        public BluetoothSocket Socket =>
            m_socket;

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

            if (!IsBluetoothAvailable)
            {
                ConsoleLogger.Warning(
                    "Bluetooth adapter not available.");

                return false;
            }

            try
            {
                Disconnect();

                ConsoleLogger.Log(
                    $"Connecting to {device.Name}...");

                m_socket =
                    device.CreateRfcommSocketToServiceRecord(
                        SerialPortUuid);

                m_adapter.CancelDiscovery();

                await m_socket.ConnectAsync();

                if (!m_socket.IsConnected)
                {
                    ConsoleLogger.Warning(
                        "Bluetooth connection failed.");

                    Disconnect();

                    return false;
                }

                ConsoleLogger.Log(
                    $"Connected to {device.Name}");

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

        public void Disconnect()
        {
            lock (m_syncRoot)
            {
                try
                {
                    m_socket?.Close();
                }
                catch
                {
                }

                try
                {
                    m_socket?.Dispose();
                }
                catch
                {
                }

                m_socket = null;
            }

            ConsoleLogger.Log(
                "Bluetooth disconnected.");
        }

        //---------------------------------------------------------------------
        // Streams
        //---------------------------------------------------------------------

        public Stream GetInputStream()
        {
            return m_socket?.InputStream;
        }

        //---------------------------------------------------------------------

        public Stream GetOutputStream()
        {
            return m_socket?.OutputStream;
        }
    }
}