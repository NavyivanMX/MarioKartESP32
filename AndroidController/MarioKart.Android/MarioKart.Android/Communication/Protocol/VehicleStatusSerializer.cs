/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : VehicleStatusSerializer.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Serializa y deserializa VehicleStatus.
 ******************************************************************************/

using MarioKart.Android.Shared;

namespace MarioKart.Android.Communication.Protocol
{
    public static class VehicleStatusSerializer
    {
        //=====================================================================
        // Deserialización
        //=====================================================================

        public static VehicleStatus Deserialize(
            byte[] payload)
        {
            if (payload == null)
            {
                return null;
            }

            //-------------------------------------------------------------
            // El payload actual contiene únicamente DrivingProfile
            //-------------------------------------------------------------

            if (payload.Length < 1)
            {
                return null;
            }

            return new VehicleStatus
            {
                DrivingProfile =
                    (DrivingProfile)payload[0]
            };
        }

        //=====================================================================
        // Serialización (por si luego la necesitamos)
        //=====================================================================

        public static byte[] Serialize(
            VehicleStatus status)
        {
            if (status == null)
            {
                return null;
            }

            return new byte[]
            {
                (byte)status.DrivingProfile
            };
        }
    }
}
