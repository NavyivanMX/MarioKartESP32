/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothTransport.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Transporte Bluetooth.
 *
 * Únicamente envía y recibe bytes.
 ******************************************************************************/

using System;
using System.IO;
using System.Threading.Tasks;

using MarioKart.Android.Debug;

namespace MarioKart.Android.Communication.Bluetooth
{
    public sealed class BluetoothTransport        : ICommunicationTransport
    {
        //=====================================================================
        // Campos
        //=====================================================================

        private readonly BluetoothConnection m_connection;

        //=====================================================================
        // Constructor
        //=====================================================================

        public BluetoothTransport(
            BluetoothConnection connection)
        {
            m_connection =
                connection
                ?? throw new ArgumentNullException(
                    nameof(connection));
        }

        //=====================================================================
        // Estado
        //=====================================================================

        public bool IsConnected =>
            m_connection.IsConnected;

        //=====================================================================
        // Envío
        //=====================================================================

        public async Task SendAsync(
            byte[] buffer)
        {
            if (buffer == null)
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
                m_connection.GetOutputStream();

            if (stream == null)
            {
                return;
            }

            try
            {
                await stream.WriteAsync(
                    buffer,
                    0,
                    buffer.Length);

                await stream.FlushAsync();

                ConsoleLogger.Log(
                    $"TX [{buffer.Length}] : {BitConverter.ToString(buffer)}");
            }
            catch (Exception ex)
            {
                ConsoleLogger.Exception(ex);

                m_connection.Disconnect();
            }
        }

        //=====================================================================
        // Recepción
        //=====================================================================

        public async Task<int> ReceiveAsync(
            byte[] buffer)
        {
            if (buffer == null)
            {
                return 0;
            }

            if (!IsConnected)
            {
                return 0;
            }

            Stream stream =
                m_connection.GetInputStream();

            if (stream == null)
            {
                return 0;
            }

            try
            {
                int bytesRead =
                    await stream.ReadAsync(
                        buffer,
                        0,
                        buffer.Length);

                if (bytesRead > 0)
                {
                    ConsoleLogger.Log(
                        $"RX [{bytesRead}] : {BitConverter.ToString(buffer, 0, bytesRead)}");
                }

                return bytesRead;
            }
            catch (Exception ex)
            {
                ConsoleLogger.Exception(ex);

                m_connection.Disconnect();

                return 0;
            }
        }
    }
}