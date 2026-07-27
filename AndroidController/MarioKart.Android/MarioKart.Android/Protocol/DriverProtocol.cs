/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DriverProtocol.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Define el formato binario del protocolo DriverCommand.
 ******************************************************************************/

namespace MarioKart.Android.Protocol
{
    public static class DriverProtocol
    {
        //=====================================================================
        // Protocolo
        //=====================================================================

        /// Primer byte mágico ('M').
        public const byte MagicByte1 = 0x4D;

        /// Segundo byte mágico ('K').
        public const byte MagicByte2 = 0x4B;

        /// Versión del protocolo.
        public const byte ProtocolVersion = 0x01;

        /// Tamaño total del paquete.
        public const int PacketSize = 8;

        //=====================================================================
        // Índices
        //=====================================================================

        public const int MagicByte1Index = 0;

        public const int MagicByte2Index = 1;

        public const int VersionIndex = 2;

        public const int DirectionIndex = 3;

        public const int SteeringIndex = 4;

        public const int TurboIndex = 5;

        public const int DriveModeIndex = 6;

        public const int ChecksumIndex = 7;
    }
}