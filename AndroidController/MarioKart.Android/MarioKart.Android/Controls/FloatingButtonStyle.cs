using Android.Graphics;

namespace MarioKart.Android.Controls
{
    /// <summary>
    /// Define la apariencia visual de un FloatingButton.
    /// </summary>
    public sealed class FloatingButtonStyle
    {
        //---------------------------------------------------------
        // Colores
        //---------------------------------------------------------

        public Color BorderColor { get; set; }

        public Color ShadowColor { get; set; }

        public Color GlowColor { get; set; }

        //---------------------------------------------------------
        // Dimensiones
        //---------------------------------------------------------

        public float BorderThickness { get; set; }

        public float ShadowRadius { get; set; }

        public float ShadowOffsetY { get; set; }

        public float GlowRadius { get; set; }

        //---------------------------------------------------------
        // Animación
        //---------------------------------------------------------

        public float PressScale { get; set; }

        public int AnimationDuration { get; set; }

        //---------------------------------------------------------
        // Constructor
        //---------------------------------------------------------

        public FloatingButtonStyle()
        {
            BorderColor = Color.White;

            ShadowColor =
                Color.Argb(
                    140,
                    0,
                    0,
                    0);

            GlowColor =
                Color.Transparent;

            BorderThickness = 6f;

            ShadowRadius = 24f;

            ShadowOffsetY = 12f;

            GlowRadius = 0f;

            PressScale = 0.92f;

            AnimationDuration = 80;
        }
    }
}