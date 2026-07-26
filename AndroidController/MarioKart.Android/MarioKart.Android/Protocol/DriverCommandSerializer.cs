/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DriverCommandSerializer.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Convierte un DriverCommand al formato binario definido por DriverProtocol.
 ******************************************************************************/

namespace MarioKart.Android.Protocol
{

    public static class DriverCommandSerializer
    {
        //=========================================================================
        // Serialización
        //=========================================================================

        public static byte[] Serialize(
            DriverCommand command)
        {
            var packet = new byte[DriverProtocol.PacketSize];

            packet[DriverProtocol.HeaderIndex] =
                DriverProtocol.Header;

            packet[DriverProtocol.VersionIndex] =
                DriverProtocol.Version;

            packet[DriverProtocol.DirectionIndex] =
                (byte)command.Direction;

            packet[DriverProtocol.SteeringIndex] =
                (byte)command.Steering;

            packet[DriverProtocol.TurboIndex] =
                command.Turbo
                    ? (byte)1
                    : (byte)0;

            packet[DriverProtocol.ChecksumIndex] =
                CalculateChecksum(packet);

            return packet;
        }

        //=========================================================================
        // Checksum
        //=========================================================================

        private static byte CalculateChecksum(
            byte[] packet)
        {
            byte checksum = 0;

            for (int i = 0; i < DriverProtocol.ChecksumIndex; i++)
            {
                checksum ^= packet[i];
            }

            return checksum;
        }
    }

}