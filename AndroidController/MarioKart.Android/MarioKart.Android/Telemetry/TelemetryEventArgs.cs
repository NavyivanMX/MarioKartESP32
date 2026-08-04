/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : TelemetryEventArgs.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Información asociada al evento de recepción de telemetría.
 ******************************************************************************/

using System;

using MarioKart.Android.Communication.Protocol;

namespace MarioKart.Android.Telemetry
{
    public sealed class TelemetryEventArgs : EventArgs
    {
        public TelemetryEventArgs(
            VehicleStatus vehicleStatus)
        {
            VehicleStatus = vehicleStatus;
        }

        /// <summary>
        /// Último estado recibido desde el Receiver.
        /// </summary>
        public VehicleStatus VehicleStatus
        {
            get;
        }
    }
}