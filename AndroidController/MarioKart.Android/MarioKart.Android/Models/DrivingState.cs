/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DrivingState.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Representa el estado actual del vehículo.
 ******************************************************************************/

using MarioKart.Android.Shared;

namespace MarioKart.Android.Models
{
    public sealed class DrivingState
    {
        //---------------------------------------------------------------------
        // Dirección longitudinal
        //---------------------------------------------------------------------

        public Direction Direction
        {
            get;
            set;
        }
        = Direction.Stop;

        //---------------------------------------------------------------------
        // Dirección lateral
        //---------------------------------------------------------------------

        public Steering Steering
        {
            get;
            set;
        }
        = Steering.Center;

        //---------------------------------------------------------------------
        // Turbo
        //---------------------------------------------------------------------

        public Turbo Turbo
        {
            get;
            set;
        }
        = Turbo.Disabled;

        //---------------------------------------------------------------------
        // Modo de conducción
        //---------------------------------------------------------------------

        public DriveMode DriveMode
        {
            get;
            set;
        }
        = DriveMode.Normal;
    }
}