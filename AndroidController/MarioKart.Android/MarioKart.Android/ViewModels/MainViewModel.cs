/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MainViewModel.cs
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * ViewModel principal de la aplicación.
 ******************************************************************************/

using MarioKart.Android.Communication;
using MarioKart.Android.Communication.Telemetry;
using MarioKart.Android.Shared;
using MarioKart.Android.Telemetry;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace MarioKart.Android.ViewModels
{
    public sealed class MainViewModel
        : INotifyPropertyChanged
    {
        //=====================================================================
        // Campos
        //=====================================================================

        private readonly CommunicationManager m_communicationManager;

        private DrivingProfile m_currentDrivingProfile;

        //=====================================================================
        // Constructor
        //=====================================================================

        public MainViewModel()
        {
            m_communicationManager =
                new CommunicationManager();

            m_communicationManager
                .Telemetry
                .VehicleStatusReceived +=
                OnVehicleStatusReceived;
        }

        //=====================================================================
        // Propiedades
        //=====================================================================

        public DrivingProfile CurrentDrivingProfile
        {
            get => m_currentDrivingProfile;

            private set
            {
                if (m_currentDrivingProfile == value)
                {
                    return;
                }

                m_currentDrivingProfile = value;

                OnPropertyChanged();
            }
        }

        public CommunicationManager Communication =>
            m_communicationManager;

        //=====================================================================
        // Eventos
        //=====================================================================

        public event PropertyChangedEventHandler
            PropertyChanged;

        //=====================================================================
        // Telemetría
        //=====================================================================

        private void OnVehicleStatusReceived(
            object sender,
            TelemetryEventArgs e)
        {
            if (e?.VehicleStatus == null)
            {
                return;
            }

            CurrentDrivingProfile =
                e.VehicleStatus.DrivingProfile;
        }

        //=====================================================================
        // Utilidades
        //=====================================================================

        private void OnPropertyChanged(
            [CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(
                this,
                new PropertyChangedEventArgs(
                    propertyName));
        }
    }
}