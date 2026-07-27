using MarioKart.Android.Communication;
using MarioKart.Android.Debug;
using MarioKart.Android.Models;
using MarioKart.Android.Protocol;
using MarioKart.Android.Shared;
using System;
using System.Diagnostics;
using System.Threading.Tasks;

namespace MarioKart.Android.Controllers
{
    /// <summary>
    /// Convierte el estado de la interfaz en un DriverCommand
    /// y lo envía mediante el transporte activo.
    /// </summary>
    public sealed class DrivingController
    {
        //---------------------------------------------------------------------
        // Campos
        //---------------------------------------------------------------------

        private readonly CommunicationManager communicationManager;

        private readonly byte[] packet =
            new byte[DriverProtocol.PacketSize];

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public DrivingController(
            CommunicationManager communicationManager)
        {
            this.communicationManager = communicationManager;
        }

        //---------------------------------------------------------------------
        // Estado
        //---------------------------------------------------------------------

        public DrivingState State { get; } =
            new DrivingState();

        public DriverCommand CurrentCommand { get; } =
            new DriverCommand();

        //---------------------------------------------------------------------
        // Actualización
        //---------------------------------------------------------------------

        public async Task UpdateAsync()
        {
            //---------------------------------------------------------
            // Dirección longitudinal
            //---------------------------------------------------------

            if (State.Forward)
            {
                CurrentCommand.Direction =
                    Direction.Forward;
            }
            else if (State.Reverse)
            {
                CurrentCommand.Direction =
                    Direction.Reverse;
            }
            else
            {
                CurrentCommand.Direction =
                    Direction.Stop;
            }

            //---------------------------------------------------------
            // Dirección lateral
            //---------------------------------------------------------

            if (State.Left)
            {
                CurrentCommand.Steering =
                    Steering.Left;
            }
            else if (State.Right)
            {
                CurrentCommand.Steering =
                    Steering.Right;
            }
            else
            {
                CurrentCommand.Steering =
                    Steering.Straight;
            }

            //---------------------------------------------------------
            // Turbo
            //---------------------------------------------------------

            CurrentCommand.Turbo =
                State.Turbo
                    ? Turbo.Enabled
                    : Turbo.Disabled;

            //---------------------------------------------------------
            // Drive Mode
            //---------------------------------------------------------

            CurrentCommand.DriveMode =
                State.Gravity
                    ? DriveMode.Gravity
                    : DriveMode.Normal;

            //---------------------------------------------------------
            // Serializar
            //---------------------------------------------------------

            if (!DriverCommandSerializer.Serialize(
                    CurrentCommand,
                    packet))
            {
                return;
            }

            //---------------------------------------------------------
            // Debug
            //---------------------------------------------------------

            ConsoleLogger.Log(
                $"TX: {BitConverter.ToString(packet)}");

            //---------------------------------------------------------
            // Enviar
            //---------------------------------------------------------

            await communicationManager.SendAsync(packet);
        }
    }
}