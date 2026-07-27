namespace MarioKart.Android.Shared
{
    /// <summary>
    /// Modo de conducción del vehículo.
    /// Debe mantenerse sincronizado con MKShared.
    /// </summary>
    public enum DriveMode : byte
    {
        /// <summary>
        /// Conducción terrestre.
        /// </summary>
        Normal = 0,

        /// <summary>
        /// Conducción antigravedad.
        /// </summary>
        Gravity = 1
    }
}