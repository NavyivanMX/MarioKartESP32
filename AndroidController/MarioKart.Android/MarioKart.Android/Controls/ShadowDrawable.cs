/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ShadowDrawable.cs
 *
 * Descripción:
 * Renderiza la sombra utilizada por FloatingButton.
 ******************************************************************************/

using Android.Graphics;
using Android.Graphics.Drawables;

namespace MarioKart.Android.Controls
{
    /// <summary>
    /// Dibuja una sombra circular suave.
    /// </summary>
    public sealed class ShadowDrawable : Drawable
    {
        //---------------------------------------------------------
        // Campos
        //---------------------------------------------------------

        private readonly Paint m_paint;

        //---------------------------------------------------------
        // Constructor
        //---------------------------------------------------------

        public ShadowDrawable()
        {
            m_paint = new Paint();

            m_paint.AntiAlias = true;

            ShadowColor =
                Color.Argb(
                    140,
                    0,
                    0,
                    0);

            Radius = 24f;

            OffsetX = 0f;

            OffsetY = 12f;
        }

        //---------------------------------------------------------
        // Propiedades
        //---------------------------------------------------------

        public Color ShadowColor
        {
            get;
            set;
        }

        public float Radius
        {
            get;
            set;
        }

        public float OffsetX
        {
            get;
            set;
        }

        public float OffsetY
        {
            get;
            set;
        }

        //---------------------------------------------------------

        public override void Draw(
            Canvas canvas)
        {
            Rect bounds = Bounds;

            float centerX =
                bounds.ExactCenterX();

            float centerY =
                bounds.ExactCenterY();

            float radius =
                System.Math.Min(
                    bounds.Width(),
                    bounds.Height()) * 0.40f;

            m_paint.Color =
                ShadowColor;

            m_paint.SetShadowLayer(
                Radius,
                OffsetX,
                OffsetY,
                ShadowColor);

            canvas.DrawCircle(
                centerX,
                centerY,
                radius,
                m_paint);
        }

        //---------------------------------------------------------

        public override void SetAlpha(
            int alpha)
        {
            m_paint.Alpha = alpha;
        }

        //---------------------------------------------------------

        public override void SetColorFilter(
            ColorFilter colorFilter)
        {
            m_paint.SetColorFilter(
                colorFilter);
        }

        //---------------------------------------------------------

        public override int Opacity =>
            (int)Format.Translucent;
    }
}