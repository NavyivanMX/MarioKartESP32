/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : FloatingButtonAnimator.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Encapsula todas las animaciones utilizadas por FloatingButton.
 ******************************************************************************/

using Android.Animation;
using Android.OS;
using Android.Views;

namespace MarioKart.Android.Controls
{
    /// <summary>
    /// Administra las animaciones de un FloatingButton.
    /// </summary>
    public sealed class FloatingButtonAnimator
    {
        //---------------------------------------------------------------------
        // Campos
        //---------------------------------------------------------------------

        private readonly View m_view;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public FloatingButtonAnimator(
            View view)
        {
            m_view = view;
        }

        //---------------------------------------------------------------------
        // Configuración
        //---------------------------------------------------------------------

        public float PressScale { get; set; } = 0.92f;

        public float PressTranslationY { get; set; } = 8f;

        public float RestTranslationY { get; set; } = 0f;

        public float RestScale { get; set; } = 1.0f;

        public float PressElevation { get; set; } = 8f;

        public float RestElevation { get; set; } = 18f;

        public int Duration { get; set; } = 80;

        //---------------------------------------------------------------------
        // API
        //---------------------------------------------------------------------

        public void AnimatePressed()
        {
            AnimateScale(PressScale);

            AnimateTranslationY(PressTranslationY);

            AnimateElevation(PressElevation);
        }

        //---------------------------------------------------------------------

        public void AnimateReleased()
        {
            AnimateScale(RestScale);

            AnimateTranslationY(RestTranslationY);

            AnimateElevation(RestElevation);
        }

        //---------------------------------------------------------------------
        // Animaciones privadas
        //---------------------------------------------------------------------

        private void AnimateScale(
            float scale)
        {
            ObjectAnimator.OfFloat(
                m_view,
                "scaleX",
                scale)
                .SetDuration(Duration)
                .Start();

            ObjectAnimator.OfFloat(
                m_view,
                "scaleY",
                scale)
                .SetDuration(Duration)
                .Start();
        }

        //---------------------------------------------------------------------

        private void AnimateTranslationY(
            float value)
        {
            ObjectAnimator.OfFloat(
                m_view,
                "translationY",
                value)
                .SetDuration(Duration)
                .Start();
        }

        //---------------------------------------------------------------------

        private void AnimateElevation(
            float value)
        {
            if (Build.VERSION.SdkInt <
                BuildVersionCodes.Lollipop)
            {
                return;
            }

            ObjectAnimator.OfFloat(
                m_view,
                "translationZ",
                value)
                .SetDuration(Duration)
                .Start();
        }

        //---------------------------------------------------------------------
        // Animaciones futuras
        //---------------------------------------------------------------------

        /// <summary>
        /// Animación de respiración para el Glow.
        /// (Se implementará posteriormente.)
        /// </summary>
        public void StartGlowPulse()
        {
        }

        //---------------------------------------------------------------------

        public void StopGlowPulse()
        {
        }

        //---------------------------------------------------------------------

        /// <summary>
        /// Animación utilizada cuando se conecta el Bluetooth.
        /// </summary>
        public void AnimateConnection()
        {
        }

        //---------------------------------------------------------------------

        /// <summary>
        /// Animación utilizada cuando el Turbo entra en modo Ready.
        /// </summary>
        public void AnimateTurboReady()
        {
        }
    }
}