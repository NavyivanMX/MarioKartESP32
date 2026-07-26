/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DriverProtocol.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Definición del protocolo binario utilizado para la comunicación entre
 * Android y el Transmitter ESP32.
 ******************************************************************************/

namespace MarioKart.Android.Protocol
{

    public static class DriverProtocol
    {
        //=====================================================================
        // Protocolo
        //=====================================================================

        /// Cabecera del paquete ('M').
        public const byte Header = 0x4D;

        /// Versión del protocolo.
        public const byte Version = 0x01;

        /// Tamaño del paquete.
        public const int PacketSize = 6;

        //=====================================================================
        // Índices
        //=====================================================================

        public const int HeaderIndex = 0;
        public const int VersionIndex = 1;
        public const int DirectionIndex = 2;
        public const int SteeringIndex = 3;
        public const int TurboIndex = 4;
        public const int ChecksumIndex = 5;
    }

}