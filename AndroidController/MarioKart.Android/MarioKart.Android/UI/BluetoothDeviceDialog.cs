/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothDeviceDialog.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Muestra un diálogo para seleccionar un dispositivo Bluetooth
 * previamente emparejado.
 ******************************************************************************/

using System.Linq;
using System.Threading.Tasks;

using Android.App;

using MarioKart.Android.Communication.Bluetooth;

namespace MarioKart.Android.UI
{
    public sealed class BluetoothDeviceDialog
    {
        //---------------------------------------------------------------------
        // Campos
        //---------------------------------------------------------------------

        private readonly Activity m_activity;

        private readonly BluetoothDiscovery m_discovery;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public BluetoothDeviceDialog(
            Activity activity,
            BluetoothDiscovery discovery)
        {
            m_activity = activity;
            m_discovery = discovery;
        }

        //---------------------------------------------------------------------
        // Selección de dispositivo
        //---------------------------------------------------------------------

        public Task<BluetoothDeviceInfo> SelectDeviceAsync()
        {
            var tcs =
                new TaskCompletionSource<BluetoothDeviceInfo>();

            var devices =
                m_discovery.GetPairedDevices();

            //-------------------------------------------------------------
            // No existen dispositivos emparejados
            //-------------------------------------------------------------

            if (devices == null || devices.Count == 0)
            {
                tcs.SetResult(null);

                return tcs.Task;
            }

            //-------------------------------------------------------------
            // Lista de nombres
            //-------------------------------------------------------------

            string[] names =
                devices
                    .Select(device => device.ToString())
                    .ToArray();

            //-------------------------------------------------------------
            // Dialogo
            //-------------------------------------------------------------

            AlertDialog.Builder builder =
                new AlertDialog.Builder(m_activity);

            builder.SetTitle(
                "Seleccionar dispositivo Bluetooth");

            builder.SetItems(
                names,
                (sender, args) =>
                {
                    tcs.TrySetResult(
                        devices[args.Which]);
                });

            builder.SetNegativeButton(
                "Cancelar",
                (sender, args) =>
                {
                    tcs.TrySetResult(null);
                });

            builder.SetCancelable(true);

            builder.Show();

            return tcs.Task;
        }
    }
}