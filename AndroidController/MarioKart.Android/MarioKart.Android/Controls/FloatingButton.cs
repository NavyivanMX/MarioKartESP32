/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : FloatingButton.cs
 *
 * Control reutilizable para toda la interfaz MarioKart RC.
 ******************************************************************************/

using Android.Content;
using Android.Graphics;
using Android.Graphics.Drawables;
using Android.Util;
using Android.Views;
using Android.Widget;

namespace MarioKart.Android.Controls
{
    public sealed class FloatingButton : FrameLayout
    {
        //---------------------------------------------------------------------
        // Controles
        //---------------------------------------------------------------------

        private readonly ImageButton m_button;

        //---------------------------------------------------------------------
        // Componentes
        //---------------------------------------------------------------------

        private readonly FloatingButtonAnimator m_animator;

        private readonly GlowDrawable m_glowDrawable;

        private readonly ShadowDrawable m_shadowDrawable;

        //---------------------------------------------------------------------
        // Estilo
        //---------------------------------------------------------------------

        private FloatingButtonStyle m_style;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public FloatingButton(Context context)
            : base(context)
        {
            m_button = CreateButton(context);

            AddView(m_button);

            m_animator =
                new FloatingButtonAnimator(this);

            m_glowDrawable =
                new GlowDrawable();

            m_shadowDrawable =
                new ShadowDrawable();

            Style =
                FloatingButtonStyles.Primary;

            RegisterTouchEffects();
        }

        public FloatingButton(
            Context context,
            IAttributeSet attrs)
            : this(context)
        {
        }

        //---------------------------------------------------------------------
        // API
        //---------------------------------------------------------------------

        public ImageButton Button =>
            m_button;

        //---------------------------------------------------------------------

        public FloatingButtonStyle Style
        {
            get => m_style;

            set
            {
                m_style = value;

                ApplyStyle();
            }
        }

        //---------------------------------------------------------------------

        public void SetIcon(
            int drawable)
        {
            m_button.SetImageResource(drawable);
        }

        //---------------------------------------------------------------------
        // Glow
        //---------------------------------------------------------------------

        public void EnableGlow()
        {
            m_glowDrawable.Show(
                m_style.GlowColor,
                m_style.GlowRadius);

            Invalidate();
        }

        //---------------------------------------------------------------------

        public void DisableGlow()
        {
            m_glowDrawable.Hide();

            Invalidate();
        }

        //---------------------------------------------------------------------

        public void SetGlowIntensity(
            int alpha)
        {
            m_glowDrawable.SetIntensity(alpha);

            Invalidate();
        }

        //---------------------------------------------------------------------
        // Sombra
        //---------------------------------------------------------------------

        public void SetShadowDepth(
            float radius,
            float offsetY)
        {
            m_shadowDrawable.Radius = radius;

            m_shadowDrawable.OffsetY = offsetY;

            Invalidate();
        }

        //---------------------------------------------------------------------
        // Dibujado
        //---------------------------------------------------------------------

        protected override void DispatchDraw(
            Canvas canvas)
        {
            m_glowDrawable.SetBounds(
                0,
                0,
                Width,
                Height);

            m_glowDrawable.Draw(canvas);

            m_shadowDrawable.SetBounds(
                0,
                0,
                Width,
                Height);

            m_shadowDrawable.Draw(canvas);

            base.DispatchDraw(canvas);
        }

        //---------------------------------------------------------------------
        // Internos
        //---------------------------------------------------------------------

        private ImageButton CreateButton(
            Context context)
        {
            var button =
                new ImageButton(context);

            button.LayoutParameters =
                new LayoutParams(
                    LayoutParams.MatchParent,
                    LayoutParams.MatchParent);

            button.SetBackgroundColor(
                Color.Transparent);

            button.SetScaleType(
                ImageView.ScaleType.FitCenter);

            return button;
        }

        //---------------------------------------------------------------------

        private void ApplyStyle()
        {
            if (m_style == null)
                return;

            m_glowDrawable.GlowColor =
                m_style.GlowColor;

            m_glowDrawable.GlowRadius =
                m_style.GlowRadius;

            m_shadowDrawable.ShadowColor =
                m_style.ShadowColor;

            m_shadowDrawable.Radius =
                m_style.ShadowRadius;

            m_shadowDrawable.OffsetY =
                m_style.ShadowOffsetY;

            m_animator.Duration =
                m_style.AnimationDuration;

            m_animator.PressScale =
                m_style.PressScale;

            Invalidate();
        }

        //---------------------------------------------------------------------

        private void RegisterTouchEffects()
        {
            m_button.Touch += (sender, e) =>
            {
                switch (e.Event.Action)
                {
                    case MotionEventActions.Down:

                        m_animator.AnimatePressed();

                        break;

                    case MotionEventActions.Up:

                    case MotionEventActions.Cancel:

                        m_animator.AnimateReleased();

                        break;
                }
                e.Handled = false;
            };
            
        }
    }
}