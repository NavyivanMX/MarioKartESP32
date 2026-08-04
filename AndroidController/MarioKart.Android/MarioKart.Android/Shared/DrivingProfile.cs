/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DrivingProfile.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Perfiles de conducción soportados por el vehículo.
 *
 * IMPORTANTE:
 * Los valores deben permanecer sincronizados con MKShared para garantizar
 * compatibilidad entre Android y los ESP32.
 ******************************************************************************/

namespace MarioKart.Android.Shared
{
    public enum DrivingProfile : byte
    {
        Rookie = 0,

        Normal = 1,

        Advanced = 2,

        Drift = 3
    }
}