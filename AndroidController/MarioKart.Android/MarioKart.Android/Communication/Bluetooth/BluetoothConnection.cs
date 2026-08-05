/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothConnection.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Encapsula una conexión Bluetooth RFCOMM.
 *
 * Es el único responsable de administrar el BluetoothSocket y los
 * Streams asociados.
 ******************************************************************************/

using System;
using System.IO;
using System.Threading.Tasks;

using Android.Bluetooth;

namespace MarioKart.Android.Communication.Bluetooth
{
    public sealed class BluetoothConnection
    {
        //=====================================================================
        // UUID SPP
        //=====================================================================

        private static readonly Guid SppUuid =
            Guid.Parse(
                "00001101-0000-1000-8000-00805F9B34FB");

        //=====================================================================
        // Campos
        //=====================================================================

        private BluetoothSocket m_socket;

        private Stream m_inputStream;

        private Stream m_outputStream;

        //=====================================================================
        // Estado
        //=====================================================================


        public bool IsConnected =>
            (m_socket != null) &&
            m_socket.IsConnected;

        //=====================================================================
        // Conexión
        //=====================================================================

        public async Task<bool> ConnectAsync(
            BluetoothDevice device)
        {
            if (device == null)
            {
                return false;
            }

            Disconnect();

            try
            {
                m_socket =
                    device.CreateRfcommSocketToServiceRecord(
                        Java.Util.UUID.FromString(
                            SppUuid.ToString()));

                await m_socket.ConnectAsync();

                m_inputStream =
                    m_socket.InputStream;

                m_outputStream =
                    m_socket.OutputStream;

                return true;
            }
            catch
            {
                Disconnect();

                return false;
            }
        }

        //=====================================================================
        // Desconexión
        //=====================================================================

        public void Disconnect()
        {
            try
            {
                m_inputStream?.Dispose();
            }
            catch
            {
            }

            try
            {
                m_outputStream?.Dispose();
            }
            catch
            {
            }

            try
            {
                m_socket?.Close();
            }
            catch
            {
            }

            m_inputStream = null;

            m_outputStream = null;

            m_socket = null;
        }

        //=====================================================================
        // Streams
        //=====================================================================

        public Stream GetInputStream()
        {
            return m_inputStream;
        }

        public Stream GetOutputStream()
        {
            return m_outputStream;
        }
    }
}