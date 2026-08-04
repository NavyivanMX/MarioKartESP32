/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : VehicleStatus.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Representa el estado del vehículo recibido desde el Receiver mediante
 * el protocolo Bluetooth.
 ******************************************************************************/

using MarioKart.Android.Shared;

namespace MarioKart.Android.Communication.Protocol
{
    /// <summary>
    /// Estado actual del vehículo enviado por el Receiver.
    /// </summary>
    public sealed class VehicleStatus
    {
        /// <summary>
        /// Perfil de conducción actualmente activo.
        /// </summary>
        public DrivingProfile DrivingProfile
        {
            get;
            set;
        }
    }
}