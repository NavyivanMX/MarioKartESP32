namespace MarioKart.Android.Communication.Protocol
{
    /// <summary>
    /// Tipos de paquetes soportados por el protocolo.
    /// Deben coincidir exactamente con PacketType.h del Receiver.
    /// </summary>
    public enum PacketType : byte
    {
        DriverCommand = 0x01,

        VehicleStatus = 0x02,

        RequestVehicleStatus = 0x03
    }
}