/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DrivingController.cs
 ******************************************************************************/

using MarioKart.Android.Communication;
using MarioKart.Android.Models;
using MarioKart.Android.Protocol;
using MarioKart.Android.Shared;
using System.Threading.Tasks;

namespace MarioKart.Android.Controllers
{
    /// <summary>
    /// Mantiene el estado actual de conducción del vehículo y
    /// solicita el envío del DriverCommand correspondiente.
    /// </summary>
    public sealed class DrivingController
    {
        //---------------------------------------------------------------------
        // Campos
        //---------------------------------------------------------------------

        private readonly DriverCommandSender m_sender;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public DrivingController(
            CommunicationManager communicationManager)
        {
            m_sender =
                new DriverCommandSender(
                    communicationManager);
        }

        //---------------------------------------------------------------------
        // Estado
        //---------------------------------------------------------------------

        public DrivingState State { get; } =
            new DrivingState();

        //---------------------------------------------------------------------
        // Comunicación
        //---------------------------------------------------------------------

        public async Task SendCurrentStateAsync()
        {
            DriverCommand command =
                BuildCommand();

            await m_sender.SendAsync(
                command);
        }

        //---------------------------------------------------------------------
        // Conversión
        //---------------------------------------------------------------------

        private DriverCommand BuildCommand()
        {
            DriverCommand command =
                new DriverCommand();

            //-------------------------------------------------------------
            // Dirección
            //-------------------------------------------------------------

            command.Direction =
                State.Direction;

            //-------------------------------------------------------------
            // Giro
            //-------------------------------------------------------------

            command.Steering =
                State.Steering;

            //-------------------------------------------------------------
            // Turbo
            //-------------------------------------------------------------

            command.Turbo =
                State.Turbo;

            //-------------------------------------------------------------
            // DriveMode
            //-------------------------------------------------------------

            command.DriveMode =
                State.DriveMode;

            return command;
        }
    }
}