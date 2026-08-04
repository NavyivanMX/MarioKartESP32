/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Packet.cs
 *
 * Descripción:
 * Representa un paquete del protocolo Bluetooth.
 *
 * Equivalente a MK::Protocol::Packet<TPayload>.
 ******************************************************************************/

using System;

namespace MarioKart.Android.Communication.Protocol
{
    public sealed class Packet
    {
        //=====================================================================
        // Constructor
        //=====================================================================

        public Packet(
            PacketType type,
            byte[] payload)
        {
            Payload =
                payload
                ?? Array.Empty<byte>();

            Type =
                type;

            PayloadSize =
                (ushort)Payload.Length;
        }

        //=====================================================================
        // Propiedades
        //=====================================================================

        /// <summary>
        /// Tipo de paquete.
        /// </summary>
        public PacketType Type
        {
            get;
        }

        /// <summary>
        /// Tamaño del payload.
        /// Debe coincidir con PacketHeader del Receiver.
        /// </summary>
        public ushort PayloadSize
        {
            get;
        }

        /// <summary>
        /// Datos del paquete.
        /// </summary>
        public byte[] Payload
        {
            get;
        }
    }
}