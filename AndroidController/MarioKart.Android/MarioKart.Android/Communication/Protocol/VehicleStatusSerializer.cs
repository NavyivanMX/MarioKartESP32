using System;

namespace MarioKart.Android.Communication
{
    /// <summary>
    /// Serialización del estado del vehículo.
    /// </summary>
    public static class VehicleStatusSerializer
    {
        /// <summary>
        /// Tamaño del payload.
        /// </summary>
        public const int PayloadSize = 1;

        //======================================================================
        // Serialize
        //======================================================================

        public static byte[] Serialize(
            VehicleStatus status)
        {
            return new[]
            {
            status.DrivingProfile
        };
        }

        //======================================================================
        // Deserialize
        //======================================================================

        public static VehicleStatus Deserialize(
            ReadOnlySpan<byte> payload)
        {
            if (payload.Length < PayloadSize)
            {
                throw new ArgumentException(
                    "Payload inválido.");
            }

            return new VehicleStatus
            {
                DrivingProfile = payload[0]
            };
        }
    }
}