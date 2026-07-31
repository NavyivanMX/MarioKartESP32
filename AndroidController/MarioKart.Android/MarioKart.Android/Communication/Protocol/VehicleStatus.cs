namespace MarioKart.Android.Communication
{

    /// <summary>
    /// Estado actual del vehículo enviado por el Receiver.
    /// </summary>
    public sealed class VehicleStatus
    {
        /// <summary>
        /// Perfil de conducción activo.
        /// Coincide con DrivingProfileId del ESP32.
        /// </summary>
        public byte DrivingProfile { get; set; }
    }
}