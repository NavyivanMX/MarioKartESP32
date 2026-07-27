using System.Threading.Tasks;

namespace MarioKart.Android.Communication
{
    /// <summary>
    /// Define el contrato para cualquier medio de comunicación
    /// utilizado por la aplicación.
    /// </summary>
    public interface ICommunicationTransport
    {
        //---------------------------------------------------------------------
        // Estado
        //---------------------------------------------------------------------

        /// <summary>
        /// Indica si el transporte se encuentra conectado.
        /// </summary>
        bool IsConnected { get; }

        //---------------------------------------------------------------------
        // Ciclo de vida
        //---------------------------------------------------------------------

        /// <summary>
        /// Finaliza la conexión con el dispositivo remoto.
        /// </summary>
        Task DisconnectAsync();

        //---------------------------------------------------------------------
        // Comunicación
        //---------------------------------------------------------------------

        /// <summary>
        /// Envía un bloque de datos.
        /// </summary>
        Task SendAsync(byte[] data);

        /// <summary>
        /// Recibe un bloque de datos.
        /// Devuelve la cantidad de bytes recibidos.
        /// </summary>
        Task<int> ReceiveAsync(byte[] buffer);
    }
}