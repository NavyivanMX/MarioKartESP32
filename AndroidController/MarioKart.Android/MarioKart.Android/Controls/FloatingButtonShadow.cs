/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : FloatingButtonShadow.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Dibuja la sombra inferior utilizada por FloatingButton.
 ******************************************************************************/

using Android.Graphics;
using Android.Graphics.Drawables;

namespace MarioKart.Android.Controls
{
    /// <summary>
    /// Dibuja una sombra circular suave debajo del botón.
    /// </summary>
    public sealed class FloatingButtonShadow
        : Drawable
    {
        //---------------------------------------------------------------------
        // Campos
        //---------------------------------------------------------------------

        private readonly Paint m_paint;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public FloatingButtonShadow()
        {
            m_paint = new Paint();

            m_paint.AntiAlias = true;

            ShadowColor =
                Color.Argb(
                    130,
                    0,
                    0,
                    0);

            ShadowRadius = 24f;

            OffsetX = 0f;

            OffsetY = 12f;

            Alpha = 255;
        }

        //---------------------------------------------------------------------
        // Propiedades
        //---------------------------------------------------------------------

        public Color ShadowColor
        {
            get;
            set;
        }

        public float ShadowRadius
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

        //---------------------------------------------------------------------

        public new int Alpha
        {
            get;
            set;
        }

        //---------------------------------------------------------------------

        public bool Enabled
        {
            get;
            set;
        } = true;

        //---------------------------------------------------------------------
        // Drawable
        //---------------------------------------------------------------------

        public override void Draw(
            Canvas canvas)
        {
            if (!Enabled)
            {
                return;
            }

            Rect bounds = Bounds;

            float centerX =
                bounds.ExactCenterX();

            float centerY =
                bounds.ExactCenterY();

            float radius =
                System.Math.Min(
                    bounds.Width(),
                    bounds.Height()) * 0.38f;

            m_paint.Color =
                ShadowColor;

            m_paint.Alpha =
                Alpha;

            m_paint.SetShadowLayer(
                ShadowRadius,
                OffsetX,
                OffsetY,
                ShadowColor);

            canvas.DrawCircle(
                centerX,
                centerY,
                radius,
                m_paint);
        }

        //---------------------------------------------------------------------

        public override void SetAlpha(
            int alpha)
        {
            Alpha = alpha;

            InvalidateSelf();
        }

        //---------------------------------------------------------------------

        public override void SetColorFilter(
            ColorFilter colorFilter)
        {
            m_paint.SetColorFilter(
                colorFilter);
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

        public void Show()
        {
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

        public void SetDepth(
            float radius,
            float offsetY)
        {
            ShadowRadius = radius;

            OffsetY = offsetY;

            InvalidateSelf();
        }

        //---------------------------------------------------------------------

        public void SetIntensity(
            int alpha)
        {
            Alpha = alpha;

            InvalidateSelf();
        }
    }
}