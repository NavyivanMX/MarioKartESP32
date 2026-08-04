/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothPermissionManager.cs
 ******************************************************************************/

using System.Threading.Tasks;

using Android;
using Android.App;
using Android.Content.PM;
using Android.OS;

using AndroidX.Core.App;
using AndroidX.Core.Content;

namespace MarioKart.Android.Communication.Bluetooth
{
    public sealed class BluetoothPermissionManager
    {
        //---------------------------------------------------------------------
        // Constantes
        //---------------------------------------------------------------------

        public const int RequestCode = 1001;

        //---------------------------------------------------------------------
        // Campos
        //---------------------------------------------------------------------

        private readonly Activity m_activity;

        private TaskCompletionSource<bool> m_permissionRequest;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public BluetoothPermissionManager(
            Activity activity)
        {
            m_activity = activity;
        }

        //---------------------------------------------------------------------
        // Estado
        //---------------------------------------------------------------------

        public bool HasPermissions()
        {
            //-------------------------------------------------------------
            // Android 11 o inferior
            //-------------------------------------------------------------

            if (Build.VERSION.SdkInt < BuildVersionCodes.S)
            {
                return true;
            }

            //-------------------------------------------------------------
            // Android 12+
            //-------------------------------------------------------------

            return

                ContextCompat.CheckSelfPermission(

                    m_activity,

                    Manifest.Permission.BluetoothConnect)

                == Permission.Granted

                &&

                ContextCompat.CheckSelfPermission(

                    m_activity,

                    Manifest.Permission.BluetoothScan)

                == Permission.Granted;
        }

        //---------------------------------------------------------------------
        // Solicitud
        //---------------------------------------------------------------------

        public async Task<bool> RequestPermissionsAsync()
        {
            //-------------------------------------------------------------
            // No hacen falta permisos
            //-------------------------------------------------------------

            if (Build.VERSION.SdkInt < BuildVersionCodes.S)
            {
                return true;
            }

            //-------------------------------------------------------------
            // Ya concedidos
            //-------------------------------------------------------------

            if (HasPermissions())
            {
                return true;
            }

            //-------------------------------------------------------------
            // Crear Task pendiente
            //-------------------------------------------------------------

            m_permissionRequest =
                new TaskCompletionSource<bool>();

            //-------------------------------------------------------------
            // Solicitar permisos
            //-------------------------------------------------------------

            ActivityCompat.RequestPermissions(

                m_activity,

                new[]
                {
                    Manifest.Permission.BluetoothConnect,
                    Manifest.Permission.BluetoothScan
                },

                RequestCode);

            //-------------------------------------------------------------
            // Esperar respuesta
            //-------------------------------------------------------------

            return await m_permissionRequest.Task;
        }

        //---------------------------------------------------------------------
        // Callback desde MainActivity
        //---------------------------------------------------------------------

        public void CompletePermissionRequest(

            int requestCode,

            Permission[] grantResults)
        {
            if (requestCode != RequestCode)
            {
                return;
            }

            bool granted = true;

            if (grantResults == null ||
                grantResults.Length == 0)
            {
                granted = false;
            }
            else
            {
                foreach (Permission permission
                    in grantResults)
                {
                    if (permission != Permission.Granted)
                    {
                        granted = false;
                        break;
                    }
                }
            }

            m_permissionRequest?
                .TrySetResult(granted);
        }
    }
}