using MarioKart.Android.Protocol;
using System;
using System.Diagnostics;

namespace MarioKart.Android.Communication
{
    public sealed class CommunicationManager
    {
        private readonly ICommunicationTransport m_transport =    new DebugTransport();
        public void Send(
            DriverCommand command)
        {
            var data =
                DriverCommandSerializer.Serialize(command);

            m_transport.Send(data);
        }
    }
}