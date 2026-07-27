
using MarioKart.Android.Shared;

namespace MarioKart.Android.Protocol
{
    public sealed class DriverCommand
    {
        public Direction Direction { get; set; }

        public Steering Steering { get; set; }

        public DriveMode DriveMode { get; set; }

        public Turbo Turbo { get; set; }

        public override string ToString()
        {
            return $"Direction={Direction} | Steering={Steering} | Turbo={Turbo}";
        }
    }
}