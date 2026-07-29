/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ICommunicationTransport.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Define el contrato para cualquier medio de comunicación utilizado
 * por la aplicación (Bluetooth, WiFi, USB, etc.).
 ******************************************************************************/

using System.Threading.Tasks;

using Android.Bluetooth;

namespace MarioKart.Android.Communication
{
    public interface ICommunicationTransport
    {
        //---------------------------------------------------------------------
        // Estado
        //---------------------------------------------------------------------

        bool IsConnected { get; }

        //---------------------------------------------------------------------
        // Conexión
        //---------------------------------------------------------------------

        Task<bool> ConnectAsync(
            BluetoothDevice device);

        Task DisconnectAsync();

        //---------------------------------------------------------------------
        // Comunicación
        //---------------------------------------------------------------------

        Task SendAsync(
            byte[] packet);

        Task<int> ReceiveAsync(
            byte[] buffer);
    }
}