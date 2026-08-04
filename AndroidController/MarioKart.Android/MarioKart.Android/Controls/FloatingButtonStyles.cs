/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : FloatingButtonStyles.cs
 ******************************************************************************/

using Android.Graphics;

namespace MarioKart.Android.Controls
{
    /// <summary>
    /// Colección de estilos reutilizables para FloatingButton.
    /// </summary>
    public static class FloatingButtonStyles
    {
        //---------------------------------------------------------------------
        // Principal
        //---------------------------------------------------------------------

        public static FloatingButtonStyle Primary =>
            new FloatingButtonStyle
            {
                BorderColor =
                    Color.White,

                GlowColor =
                    Color.Argb(
                        120,
                        70,
                        170,
                        255),

                GlowRadius = 22f
            };

        //---------------------------------------------------------------------
        // Turbo
        //---------------------------------------------------------------------

        public static FloatingButtonStyle Turbo =>
            new FloatingButtonStyle
            {
                BorderColor =
                    Color.Rgb(
                        255,
                        220,
                        120),

                GlowColor =
                    Color.Argb(
                        180,
                        255,
                        90,
                        0),

                GlowRadius = 30f
            };

        //---------------------------------------------------------------------
        // Bluetooth
        //---------------------------------------------------------------------

        public static FloatingButtonStyle Bluetooth =>
            new FloatingButtonStyle
            {
                BorderColor =
                    Color.Rgb(
                        80,
                        180,
                        255),

                GlowColor =
                    Color.Argb(
                        180,
                        40,
                        180,
                        255),

                GlowRadius = 24f
            };

        //---------------------------------------------------------------------
        // Configuración
        //---------------------------------------------------------------------

        public static FloatingButtonStyle Settings =>
            new FloatingButtonStyle
            {
                BorderColor =
                    Color.Rgb(
                        255,
                        230,
                        120),

                GlowColor =
                    Color.Argb(
                        160,
                        255,
                        210,
                        0),

                GlowRadius = 24f
            };

        //---------------------------------------------------------------------
        // Profile
        //---------------------------------------------------------------------

        public static FloatingButtonStyle Profile =>
            new FloatingButtonStyle
            {
                BorderColor =
                    Color.Rgb(
                        255,
                        230,
                        120),

                GlowColor =
                    Color.Argb(
                        160,
                        255,
                        210,
                        0),

                GlowRadius = 24f
            };
    }
}