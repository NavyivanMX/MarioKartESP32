/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : FloatingButtonGlow.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Encapsula el efecto Glow utilizado por FloatingButton.
 ******************************************************************************/

using Android.Graphics;
using Android.Graphics.Drawables;

namespace MarioKart.Android.Controls
{
    /// <summary>
    /// Dibuja el resplandor exterior del FloatingButton.
    /// </summary>
    public sealed class FloatingButtonGlow : Drawable
    {
        //---------------------------------------------------------------------
        // Campos
        //---------------------------------------------------------------------

        private readonly Paint m_paint;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public FloatingButtonGlow()
        {
            m_paint = new Paint();

            m_paint.AntiAlias = true;

            GlowColor = Color.Transparent;

            GlowRadius = 0f;

            GlowAlpha = 255;
        }

        //---------------------------------------------------------------------
        // Propiedades
        //---------------------------------------------------------------------

        public Color GlowColor
        {
            get;
            set;
        }

        public float GlowRadius
        {
            get;
            set;
        }

        public int GlowAlpha
        {
            get;
            set;
        }

        public bool Enabled
        {
            get;
            set;
        }

        //---------------------------------------------------------------------
        // Drawable
        //---------------------------------------------------------------------

        public override void Draw(Canvas canvas)
        {
            if (!Enabled)
            {
                return;
            }

            if (GlowRadius <= 0f)
            {
                return;
            }

            Rect bounds = Bounds;

            float centerX = bounds.ExactCenterX();

            float centerY = bounds.ExactCenterY();

            float radius =
                System.Math.Min(
                    bounds.Width(),
                    bounds.Height()) * 0.42f;

            m_paint.Color =
                GlowColor;

            m_paint.Alpha =
                GlowAlpha;

            m_paint.SetShadowLayer(
                GlowRadius,
                0f,
                0f,
                GlowColor);

            canvas.DrawCircle(
                centerX,
                centerY,
                radius,
                m_paint);
        }

        //---------------------------------------------------------------------

        public override void SetAlpha(int alpha)
        {
            GlowAlpha = alpha;

            InvalidateSelf();
        }

        //---------------------------------------------------------------------

        public override void SetColorFilter(
            ColorFilter colorFilter)
        {
            m_paint.SetColorFilter(colorFilter);
        }

        //---------------------------------------------------------------------

        public override int Opacity
        {
            get
            {
                return (int)Format.Translucent;
            }
        }

        //---------------------------------------------------------------------
        // API
        //---------------------------------------------------------------------

        public void Show(
            Color color,
            float radius)
        {
            GlowColor = color;

            GlowRadius = radius;

            Enabled = true;

            InvalidateSelf();
        }

        //---------------------------------------------------------------------

        public void Hide()
        {
            Enabled = false;

            InvalidateSelf();
        }

        //---------------------------------------------------------------------

        public void SetIntensity(
            int alpha)
        {
            GlowAlpha = alpha;

            InvalidateSelf();
        }
    }
}