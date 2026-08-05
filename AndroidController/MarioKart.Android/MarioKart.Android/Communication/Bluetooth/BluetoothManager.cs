/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothManager.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Gestiona el protocolo Bluetooth.
 *
 * No conoce BluetoothSocket ni Streams.
 * Únicamente trabaja con paquetes del protocolo.
 ******************************************************************************/

using System;
using System.Threading;
using System.Threading.Tasks;

using MarioKart.Android.Communication.Protocol;
using MarioKart.Android.Debug;

namespace MarioKart.Android.Communication.Bluetooth
{
    public sealed class BluetoothManager
    {
        //=====================================================================
        // Campos
        //=====================================================================

        private readonly BluetoothTransport m_transport;

        private CancellationTokenSource m_receiveLoopCancellation;

        //=====================================================================
        // Eventos
        //=====================================================================

        public event EventHandler<Packet> PacketReceived;

        //=====================================================================
        // Constructor
        //=====================================================================

        public BluetoothManager(
            BluetoothTransport transport)
        {
            m_transport =
                transport
                ?? throw new ArgumentNullException(
                    nameof(transport));
        }

        //=====================================================================
        // Estado
        //=====================================================================

        public bool IsConnected
        {
            get
            {
                return m_transport.IsConnected;
            }
        }

        //=====================================================================
        // Recepción
        //=====================================================================

        public void Start()
        {
            if (m_receiveLoopCancellation != null)
            {
                return;
            }

            m_receiveLoopCancellation =
                new CancellationTokenSource();

            _ = ReceiveLoopAsync(
                m_receiveLoopCancellation.Token);
        }

        public void Stop()
        {
            m_receiveLoopCancellation?.Cancel();

            m_receiveLoopCancellation = null;
        }

        //=====================================================================
        // Envío
        //=====================================================================

        public async Task SendAsync(
            Packet packet)
        {
            if (packet == null)
            {
                return;
            }

            byte[] buffer =
                PacketSerializer.Serialize(packet);

            if (buffer == null)
            {
                return;
            }

            await m_transport.SendAsync(
                buffer);
        }
        //=====================================================================
        // Desconexión
        //=====================================================================

        public void Disconnect()
        {
            //-------------------------------------------------------------
            // Detener recepción
            //-------------------------------------------------------------

            Stop();

            //-------------------------------------------------------------
            // Cerrar transporte
            //-------------------------------------------------------------

            m_transport.Disconnect();
        }

        //=====================================================================
        // Receive Loop
        //=====================================================================

        private async Task ReceiveLoopAsync(
            CancellationToken cancellationToken)
        {
            byte[] buffer =
                new byte[256];

            while (!cancellationToken.IsCancellationRequested)
            {
                int count =
                    await m_transport.ReceiveAsync(
                        buffer);

                if (count <= 0)
                {
                    continue;
                }

                ProcessReceivedData(
                    buffer,
                    count);
            }
        }

        //=====================================================================
        // Procesamiento
        //=====================================================================

        private void ProcessReceivedData(
            byte[] buffer,
            int length)
        {
            Packet packet =
                PacketSerializer.Deserialize(
                    buffer.AsSpan(0, length));

            if (packet == null)
            {
                return;
            }
            PacketReceived?.Invoke(
                this,
                packet);
        }
    }
}