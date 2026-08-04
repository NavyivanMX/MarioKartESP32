/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ICommunicationTransport.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Define el contrato para cualquier transporte de comunicación.
 *
 * Un transporte únicamente envía y recibe bytes.
 ******************************************************************************/

using System.Threading.Tasks;

namespace MarioKart.Android.Communication
{
    public interface ICommunicationTransport
    {
        //=====================================================================
        // Estado
        //=====================================================================

        bool IsConnected
        {
            get;
        }

        //=====================================================================
        // Envío
        //=====================================================================

        Task SendAsync(
            byte[] buffer);

        //=====================================================================
        // Recepción
        //=====================================================================

        Task<int> ReceiveAsync(
            byte[] buffer);
    }
}