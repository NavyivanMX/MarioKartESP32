/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : CommunicationManager.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Orquesta toda la infraestructura de comunicación.
 ******************************************************************************/

using Android.Bluetooth;
using MarioKart.Android.Communication.Bluetooth;
using MarioKart.Android.Communication.Protocol;
using MarioKart.Android.Communication.Telemetry;
using System;
using System.Threading.Tasks;
using BluetoothManager = MarioKart.Android.Communication.Bluetooth.BluetoothManager;

namespace MarioKart.Android.Communication
{
    public sealed class CommunicationManager
    {
        //=====================================================================
        // Campos
        //=====================================================================

        private readonly BluetoothConnection m_connection;

        private readonly BluetoothTransport m_transport;

        private readonly BluetoothManager m_bluetoothManager;

        private readonly TelemetryManager m_telemetryManager;

        public TelemetryManager TelemetryManager
        {
            get
            {
                return m_telemetryManager;
            }
        }

        //=====================================================================
        // Constructor
        //=====================================================================

        public CommunicationManager()
        {
            m_connection =
                new BluetoothConnection();

            m_transport =
                new BluetoothTransport(
                    m_connection);

            m_bluetoothManager =
                new BluetoothManager(
                    m_transport);

            m_telemetryManager =
                new TelemetryManager(
                    m_bluetoothManager);
        }

        //=====================================================================
        // Estado
        //=====================================================================

        public bool IsConnected =>
            m_connection.IsConnected;

        public TelemetryManager Telemetry =>
            m_telemetryManager;
        //---------------------------------------------------------------------
        // Envío de paquetes
        //---------------------------------------------------------------------

        public async Task SendAsync(
            Packet packet)
        {
            if (packet == null)
            {
                return;
            }

            await m_bluetoothManager.SendAsync(
                packet);
        }

        //=====================================================================
        // Conexión
        //=====================================================================

        public async Task<bool> ConnectAsync(
            BluetoothDevice device)
        {
            bool connected =
                await m_connection.ConnectAsync(
                    device);

            if (connected)
            {
                m_bluetoothManager.Start();
            }

            return connected;
        }

        //=====================================================================
        // Desconexión
        //=====================================================================

        public void Disconnect()
        {
            m_bluetoothManager.Stop();

            m_connection.Disconnect();
        }
    }
}