/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DriverCommandSender.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Convierte un DriverCommand en un paquete binario y lo envía utilizando
 * el CommunicationManager.
 ******************************************************************************/

using System.Threading.Tasks;

using MarioKart.Android.Protocol;
using MarioKart.Android.Shared;

namespace MarioKart.Android.Communication
{
    /// <summary>
    /// Encapsula el proceso completo de serialización y envío
    /// de DriverCommand.
    /// </summary>
    public sealed class DriverCommandSender
    {
        //---------------------------------------------------------------------
        // Campos
        //---------------------------------------------------------------------

        private readonly CommunicationManager m_communication;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public DriverCommandSender(
            CommunicationManager communication)
        {
            m_communication = communication;
        }

        //---------------------------------------------------------------------
        // Envío
        //---------------------------------------------------------------------

        /// <summary>
        /// Serializa y envía el DriverCommand actual.
        /// </summary>
        public async Task<bool> SendAsync(
            DriverCommand command)
        {
            if (m_communication == null)
            {
                return false;
            }

            if (!m_communication.IsConnected)
            {
                return false;
            }

            byte[] packet =
                new byte[
                    DriverCommandSerializer.PacketSize];

            if (!DriverCommandSerializer.Serialize(
                    command,
                    packet))
            {
                return false;
            }

            await m_communication.SendAsync(
                packet);

            return true;
        }
    }
}