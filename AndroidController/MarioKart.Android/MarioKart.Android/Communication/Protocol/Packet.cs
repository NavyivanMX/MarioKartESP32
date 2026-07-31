/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Packet.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Representa un paquete del protocolo de comunicación entre Android
 * y el Receiver.
 ******************************************************************************/

using System;

namespace MarioKart.Android.Communication.Protocol
{

    /// <summary>
    /// Cabecera común para todos los paquetes.
    /// El payload se serializa por separado.
    /// </summary>
    public sealed class Packet
    {
        /// <summary>
        /// Tipo de paquete.
        /// </summary>
        public PacketType Type { get; set; }

        /// <summary>
        /// Longitud del payload.
        /// </summary>
        public ushort PayloadLength { get; set; }

        /// <summary>
        /// Datos serializados.
        /// </summary>
        public byte[] Payload { get; set; } = System.Array.Empty<byte>();
    }
}