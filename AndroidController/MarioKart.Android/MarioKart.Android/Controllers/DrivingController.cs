using System.Diagnostics;
using MarioKart.Android.Models;
using MarioKart.Android.Protocol;
using MarioKart.Android.Communication;

namespace MarioKart.Android.Controllers
{
    public sealed class DrivingController
    {
        public DrivingState State { get; } =            new DrivingState();

        public DriverCommand CurrentCommand { get; } =            new DriverCommand();

        private readonly CommunicationManager m_communication =    new CommunicationManager();
        public void Update()
        {
            //---------------------------------------------------------
            // Dirección
            //---------------------------------------------------------

            if (State.Forward)
            {
                CurrentCommand.Direction = Direction.Forward;
            }
            else if (State.Reverse)
            {
                CurrentCommand.Direction = Direction.Reverse;
            }
            else
            {
                CurrentCommand.Direction = Direction.Stop;
            }

            //---------------------------------------------------------
            // Dirección lateral
            //---------------------------------------------------------

            if (State.Left)
            {
                CurrentCommand.Steering = Steering.Left;
            }
            else if (State.Right)
            {
                CurrentCommand.Steering = Steering.Right;
            }
            else
            {
                CurrentCommand.Steering = Steering.Straight;
            }

            //---------------------------------------------------------
            // Turbo
            //---------------------------------------------------------

            CurrentCommand.Turbo = State.Turbo;

            m_communication.Send(CurrentCommand);
        }
    }
}