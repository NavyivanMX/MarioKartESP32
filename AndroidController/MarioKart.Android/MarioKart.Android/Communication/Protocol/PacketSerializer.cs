/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : PacketSerializer.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Serializa y deserializa paquetes del protocolo de comunicación entre
 * Android y el Receiver.
 ******************************************************************************/

using System;

namespace MarioKart.Android.Communication.Protocol
{

    public static class PacketSerializer
    {
        //=========================================================================
        // Serialize
        //=========================================================================

        /// <summary>
        /// Convierte un Packet a un arreglo de bytes.
        /// Formato actual:
        ///
        /// [0]      PacketType
        /// [1..2]   PayloadLength (UInt16)
        /// [3..N]   Payload
        /// </summary>
        public static byte[] Serialize(
            Packet packet)
        {
            if (packet == null)
            {
                throw new ArgumentNullException(nameof(packet));
            }

            packet.Payload ??= Array.Empty<byte>();

            packet.PayloadLength =
                (ushort)packet.Payload.Length;

            byte[] buffer =
                new byte[
                    1 +
                    sizeof(ushort) +
                    packet.PayloadLength];

            //-------------------------------------------------------------
            // PacketType
            //-------------------------------------------------------------

            buffer[0] =
                (byte)packet.Type;

            //-------------------------------------------------------------
            // PayloadLength
            //-------------------------------------------------------------

            BitConverter
                .GetBytes(packet.PayloadLength)
                .CopyTo(buffer, 1);

            //-------------------------------------------------------------
            // Payload
            //-------------------------------------------------------------

            packet.Payload.CopyTo(
                buffer,
                3);

            return buffer;
        }

        //=========================================================================
        // Deserialize
        //=========================================================================

        /// <summary>
        /// Convierte un arreglo de bytes en un Packet.
        /// </summary>
        public static Packet Deserialize(
            ReadOnlySpan<byte> buffer)
        {
            if (buffer.Length < 3)
            {
                throw new ArgumentException(
                    "Packet inválido.");
            }

            Packet packet =
                new Packet
                {
                    Type =
                        (PacketType)buffer[0],

                    PayloadLength =
                    BitConverter.ToUInt16(
                        buffer
                            .Slice(1, 2)
                            .ToArray(),
                        0),

                    Payload =
                        Array.Empty<byte>()
                };

            //-------------------------------------------------------------
            // Payload
            //-------------------------------------------------------------

            if (packet.PayloadLength > 0)
            {
                packet.Payload =
                    buffer
                        .Slice(
                            3,
                            packet.PayloadLength)
                        .ToArray();
            }

            return packet;
        }
    }

}