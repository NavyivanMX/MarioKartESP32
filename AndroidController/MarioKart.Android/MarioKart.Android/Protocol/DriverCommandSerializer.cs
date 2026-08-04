/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DriverCommandSerializer.cs
 *
 * Descripción:
 * Convierte DriverCommand en un Packet compatible con MKShared.
 ******************************************************************************/

using MarioKart.Android.Communication.Protocol;
using MarioKart.Android.Shared;

namespace MarioKart.Android.Protocol
{
    public static class DriverCommandSerializer
    {
        //=====================================================================
        // Serialización
        //=====================================================================

        public static Packet Serialize(
            DriverCommand command)
        {
            if (command == null)
            {
                return null;
            }

            //-------------------------------------------------------------
            // Payload
            //-------------------------------------------------------------

            byte[] payload =
            {
                (byte)command.Direction,
                (byte)command.Steering,
                (byte)command.Turbo,
                (byte)command.DriveMode
            };

            //-------------------------------------------------------------
            // Packet
            //-------------------------------------------------------------

            return new Packet(
                PacketType.DriverCommand,
                payload);
        }
    }
}