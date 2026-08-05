/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : TelemetryManager.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Gestiona la telemetría recibida desde el vehículo.
 *
 * Convierte paquetes Packet en objetos VehicleStatus.
 ******************************************************************************/

using MarioKart.Android.Communication.Bluetooth;
using MarioKart.Android.Communication.Protocol;
using MarioKart.Android.Debug;
using MarioKart.Android.Telemetry;
using System;

namespace MarioKart.Android.Communication.Telemetry
{
    public sealed class TelemetryManager
    {
        //=====================================================================
        // Campos
        //=====================================================================

        private readonly BluetoothManager m_bluetoothManager;

        //=====================================================================
        // Eventos
        //=====================================================================

        public event EventHandler<TelemetryEventArgs>
            VehicleStatusReceived;

        //=====================================================================
        // Constructor
        //=====================================================================

        public TelemetryManager(
            BluetoothManager bluetoothManager)
        {
            m_bluetoothManager =
                bluetoothManager
                ?? throw new ArgumentNullException(
                    nameof(bluetoothManager));

            m_bluetoothManager.PacketReceived +=
                OnPacketReceived;
        }

        //=====================================================================
        // Eventos
        //=====================================================================

        private void OnPacketReceived(
            object sender,
            Packet packet)
        {
            if (packet == null)
            {
                return;
            }

            if (packet.Type != PacketType.VehicleStatus)
            {
                return;
            }

            VehicleStatus status =
                VehicleStatusSerializer.Deserialize(
                    packet.Payload);            

            if (status == null)
            {
                return;
            }

            VehicleStatusReceived?.Invoke(
                this,
                new TelemetryEventArgs(status));

        }
    }
}