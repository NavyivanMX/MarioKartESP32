/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : CommunicationManager.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Orquesta el transporte de comunicación utilizado por la aplicación.
 ******************************************************************************/

using System;
using System.Threading.Tasks;

using Android.Bluetooth;

namespace MarioKart.Android.Communication
{
    /// <summary>
    /// Punto central de acceso al transporte de comunicación.
    /// Orquesta la conexión y el intercambio de datos sin conocer
    /// la implementación concreta del transporte.
    /// </summary>
    public sealed class CommunicationManager
    {
        //---------------------------------------------------------------------
        // Campos
        //---------------------------------------------------------------------

        private readonly ICommunicationTransport m_transport;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public CommunicationManager(
            ICommunicationTransport transport)
        {
            m_transport =
                transport
                ?? throw new ArgumentNullException(
                    nameof(transport));
        }

        //---------------------------------------------------------------------
        // Estado
        //---------------------------------------------------------------------

        public bool IsConnected =>
            m_transport.IsConnected;

        //---------------------------------------------------------------------
        // Conexión
        //---------------------------------------------------------------------

        public async Task<bool> ConnectBluetoothAsync(
            BluetoothDevice device)
        {
            if (device == null)
            {
                return false;
            }

            return await
                m_transport.ConnectAsync(device);
        }

        public async Task DisconnectAsync()
        {
            await m_transport.DisconnectAsync();
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
                return;
            }

            await m_transport.SendAsync(
                packet);
        }

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

            return await m_transport.ReceiveAsync(
                buffer);
        }
    }
}