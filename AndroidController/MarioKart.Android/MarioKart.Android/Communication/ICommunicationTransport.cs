namespace MarioKart.Android.Communication
{
    public interface ICommunicationTransport
    {
        void Send(byte[] data);
    }
}