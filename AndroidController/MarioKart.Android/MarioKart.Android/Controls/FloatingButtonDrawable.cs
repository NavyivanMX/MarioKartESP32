/******************************************************************************
 * FloatingButtonDrawable
 ******************************************************************************/

using Android.Graphics;
using Android.Graphics.Drawables;

namespace MarioKart.Android.Controls
{
    public sealed class FloatingButtonDrawable : Drawable
    {
        //------------------------------------------------------------
        // Paints
        //------------------------------------------------------------

        private readonly Paint m_fillPaint;

        private readonly Paint m_borderPaint;

        private readonly Paint m_highlightPaint;

        //------------------------------------------------------------

        public FloatingButtonDrawable()
        {
            m_fillPaint = new Paint();

            m_fillPaint.AntiAlias = true;

            m_borderPaint = new Paint();

            m_borderPaint.AntiAlias = true;

            m_borderPaint.SetStyle(Paint.Style.Stroke);

            m_highlightPaint = new Paint();

            m_highlightPaint.AntiAlias = true;

            BorderColor = Color.White;

            BaseColor =
                Color.Rgb(
                    60,
                    150,
                    255);
        }

        //------------------------------------------------------------

        public Color BaseColor
        {
            get;
            set;
        }

        //------------------------------------------------------------

        public Color BorderColor
        {
            get;
            set;
        }

        //------------------------------------------------------------

        public float BorderWidth
        {
            get;
            set;
        } = 6f;

        //------------------------------------------------------------

        public override void Draw(
            Canvas canvas)
        {
            Rect bounds = Bounds;

            float cx = bounds.ExactCenterX();

            float cy = bounds.ExactCenterY();

            float radius =
                System.Math.Min(
                    bounds.Width(),
                    bounds.Height()) * 0.42f;

            DrawGradient(
                canvas,
                cx,
                cy,
                radius);

            DrawHighlight(
                canvas,
                cx,
                cy,
                radius);

            DrawBorder(
                canvas,
                cx,
                cy,
                radius);

            DrawGloss(
                canvas,
                cx,
                cy,
                radius);
        }

        //------------------------------------------------------------

        private void DrawGradient(
            Canvas canvas,
            float cx,
            float cy,
            float radius)
        {
            RadialGradient shader =
                new RadialGradient(
                    cx,
                    cy - radius * 0.20f,
                    radius,
                    Color.White,
                    BaseColor,
                    Shader.TileMode.Clamp);

            m_fillPaint.SetShader(shader);

            canvas.DrawCircle(
                cx,
                cy,
                radius,
                m_fillPaint);

            m_fillPaint.SetShader(null);
        }

        //------------------------------------------------------------

        private void DrawBorder(
            Canvas canvas,
            float cx,
            float cy,
            float radius)
        {
            m_borderPaint.Color =
                BorderColor;

            m_borderPaint.StrokeWidth =
                BorderWidth;

            canvas.DrawCircle(
                cx,
                cy,
                radius,
                m_borderPaint);
        }

        //------------------------------------------------------------

        private void DrawHighlight(
            Canvas canvas,
            float cx,
            float cy,
            float radius)
        {
            m_highlightPaint.Color =
                Color.Argb(
                    70,
                    255,
                    255,
                    255);

            canvas.DrawCircle(
                cx,
                cy - radius * 0.45f,
                radius * 0.35f,
                m_highlightPaint);
        }

        //------------------------------------------------------------

        private void DrawGloss(
            Canvas canvas,
            float cx,
            float cy,
            float radius)
        {
            RectF rect =
                new RectF(
                    cx - radius * 0.70f,
                    cy - radius * 0.80f,
                    cx + radius * 0.70f,
                    cy - radius * 0.10f);

            Paint glossPaint =
                new Paint();

            glossPaint.AntiAlias = true;

            LinearGradient shader =
                new LinearGradient(
                    rect.Left,
                    rect.Top,
                    rect.Left,
                    rect.Bottom,
                    Color.Argb(
                        140,
                        255,
                        255,
                        255),
                    Color.Transparent,
                    Shader.TileMode.Clamp);

            glossPaint.SetShader(shader);

            canvas.DrawOval(
                rect,
                glossPaint);
        }

        //------------------------------------------------------------

        public override void SetAlpha(int alpha)
        {
        }

        //------------------------------------------------------------

        public override void SetColorFilter(
            ColorFilter colorFilter)
        {
        }

        //------------------------------------------------------------

        public override int Opacity =>
            (int)Format.Translucent;
    }
}