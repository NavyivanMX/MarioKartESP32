/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DriverCommandSerializer.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Convierte un DriverCommand al formato binario definido por DriverProtocol.
 ******************************************************************************/

using System;

using MarioKart.Android.Shared;

namespace MarioKart.Android.Protocol
{
    public static class DriverCommandSerializer
    {
        //---------------------------------------------------------------------
        // Constantes
        //---------------------------------------------------------------------

        public const int PacketSize =
            DriverProtocol.PacketSize;

        //=====================================================================
        // Serialización
        //=====================================================================

        public static bool Serialize(
            DriverCommand command,
            byte[] buffer)
        {
            if (command == null)
            {
                return false;
            }

            if (buffer == null)
            {
                return false;
            }

            if (buffer.Length < PacketSize)
            {
                return false;
            }

            //-------------------------------------------------------------
            // Encabezado
            //-------------------------------------------------------------

            buffer[DriverProtocol.MagicByte1Index] =
                DriverProtocol.MagicByte1;

            buffer[DriverProtocol.MagicByte2Index] =
                DriverProtocol.MagicByte2;

            buffer[DriverProtocol.VersionIndex] =
                DriverProtocol.ProtocolVersion;

            //-------------------------------------------------------------
            // DriverCommand
            //-------------------------------------------------------------

            buffer[DriverProtocol.DirectionIndex] =
                (byte)command.Direction;

            buffer[DriverProtocol.SteeringIndex] =
                (byte)command.Steering;

            buffer[DriverProtocol.TurboIndex] =
                (byte)command.Turbo;

            buffer[DriverProtocol.DriveModeIndex] =
                (byte)command.DriveMode;

            //-------------------------------------------------------------
            // Checksum
            //-------------------------------------------------------------

            buffer[DriverProtocol.ChecksumIndex] =
                CalculateChecksum(
                    buffer,
                    DriverProtocol.ChecksumIndex);

            return true;
        }

        //=====================================================================
        // Checksum
        //=====================================================================

        private static byte CalculateChecksum(
            byte[] buffer,
            int length)
        {
            byte checksum = 0;

            for (int i = 0; i < length; ++i)
            {
                checksum ^= buffer[i];
            }

            return checksum;
        }
    }
}