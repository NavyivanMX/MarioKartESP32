namespace MarioKart.Android.Communication.Protocol
{
    /// <summary>
    /// Tipos de paquetes soportados por el protocolo.
    /// Deben coincidir exactamente con PacketType.h del Receiver.
    /// </summary>
    public enum PacketType : byte
    {
        DriverCommand = 0,

        VehicleStatus = 1
    }
}