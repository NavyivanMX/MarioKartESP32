/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothManager.cs
 ******************************************************************************/

using System.Threading.Tasks;

namespace MarioKart.Android.Communication.Bluetooth
{

    public sealed class BluetoothManager
    {
        //---------------------------------------------------------------------
        // Estado
        //---------------------------------------------------------------------

        public bool IsConnected
        {
            get;
            private set;
        }

        //---------------------------------------------------------------------
        // Descubrimiento
        //---------------------------------------------------------------------

        public Task<BluetoothDeviceInfo[]> DiscoverAsync()
        {
            return Task.FromResult(
                new BluetoothDeviceInfo[0]);
        }

        //---------------------------------------------------------------------
        // Conexión
        //---------------------------------------------------------------------

        public Task<bool> ConnectAsync(
            BluetoothDeviceInfo device)
        {
            return Task.FromResult(false);
        }

        //---------------------------------------------------------------------
        // Desconexión
        //---------------------------------------------------------------------

        public void Disconnect()
        {
        }

        //---------------------------------------------------------------------
        // Envío
        //---------------------------------------------------------------------

        public void Send(
            byte[] data)
        {
        }
    }

}