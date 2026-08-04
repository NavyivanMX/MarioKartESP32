/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : PacketSerializer.cs
 *
 * Descripción:
 * Serializa y deserializa Packet.
 *
 * Formato:
 *
 * +------------+
 * | Type        | 1 byte
 * +------------+
 * | PayloadSize | 2 bytes
 * +------------+
 * | Payload     | N bytes
 * +------------+
 ******************************************************************************/

using System;

namespace MarioKart.Android.Communication.Protocol
{
    public static class PacketSerializer
    {
        //=====================================================================
        // Serialización
        //=====================================================================

        public static byte[] Serialize(
            Packet packet)
        {
            if (packet == null)
            {
                return null;
            }

            int packetSize =
                1 +                      // Type
                2 +                      // PayloadSize
                packet.Payload.Length;

            byte[] buffer =
                new byte[packetSize];

            //-------------------------------------------------------------
            // PacketType
            //-------------------------------------------------------------

            buffer[0] =
                (byte)packet.Type;

            //-------------------------------------------------------------
            // PayloadSize (Little Endian)
            //-------------------------------------------------------------

            BitConverter.GetBytes(
                packet.PayloadSize)
                .CopyTo(
                    buffer,
                    1);

            //-------------------------------------------------------------
            // Payload
            //-------------------------------------------------------------

            if (packet.Payload.Length > 0)
            {
                Array.Copy(
                    packet.Payload,
                    0,
                    buffer,
                    3,
                    packet.Payload.Length);
            }

            return buffer;
        }

        //=====================================================================
        // Deserialización
        //=====================================================================

        public static Packet Deserialize(
            ReadOnlySpan<byte> buffer)
        {
            //-------------------------------------------------------------
            // Header
            //-------------------------------------------------------------

            if (buffer.Length < 3)
            {
                return null;
            }

            PacketType type =
                (PacketType)buffer[0];

            ushort payloadSize =
                BitConverter.ToUInt16(
                    buffer
                        .Slice(1, 2)
                        .ToArray(),
                    0);

            //-------------------------------------------------------------
            // Validar tamaño
            //-------------------------------------------------------------

            if (buffer.Length < 3 + payloadSize)
            {
                return null;
            }

            //-------------------------------------------------------------
            // Payload
            //-------------------------------------------------------------

            byte[] payload =
                buffer
                    .Slice(
                        3,
                        payloadSize)
                    .ToArray();

            return new Packet(
                type,
                payload);
        }
    }
}