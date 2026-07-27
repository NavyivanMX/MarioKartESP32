/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ConsoleLogger.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Logger de consola para Android.
 * Centraliza todos los mensajes enviados a Logcat.
 ******************************************************************************/

using Log = global::Android.Util.Log;

namespace MarioKart.Android.Debug
{
    public static class ConsoleLogger
    {
        //---------------------------------------------------------------------
        // Configuración
        //---------------------------------------------------------------------

        private const string Tag = "MarioKart";

        #if DEBUG
                private const bool Enabled = true;
        #else
                private const bool Enabled = false;
        #endif

        //---------------------------------------------------------------------
        // Información
        //---------------------------------------------------------------------

        public static void Log(string message)
        {
            if (!Enabled)
            {
                return;
            }

            global::Android.Util.Log.Debug(Tag, message);
        }

        //---------------------------------------------------------------------
        // Advertencias
        //---------------------------------------------------------------------

        public static void Warning(string message)
        {
            if (!Enabled)
            {
                return;
            }

            global::Android.Util.Log.Warn(Tag, message);
        }

        //---------------------------------------------------------------------
        // Errores
        //---------------------------------------------------------------------

        public static void Error(string message)
        {
            if (!Enabled)
            {
                return;
            }

            global::Android.Util.Log.Error(Tag, message);
        }

        //---------------------------------------------------------------------
        // Excepciones
        //---------------------------------------------------------------------

        public static void Exception(System.Exception ex)
        {
            if (!Enabled)
            {
                return;
            }

            global::Android.Util.Log.Error(
                Tag,
                ex.ToString());
        }
    }
}