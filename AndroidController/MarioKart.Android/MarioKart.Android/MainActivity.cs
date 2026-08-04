/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MainActivity.cs
 ******************************************************************************/

using Android.App;
using Android.Content.PM;
using Android.Graphics;
using Android.OS;
using Android.Views;
using Android.Widget;
using MarioKart.Android.Communication;
using MarioKart.Android.Communication.Bluetooth;
using MarioKart.Android.Communication.Telemetry;
using MarioKart.Android.Controllers;
using MarioKart.Android.Controls;
using MarioKart.Android.Debug;
using MarioKart.Android.Protocol;
using MarioKart.Android.Shared;
using MarioKart.Android.Telemetry;
using MarioKart.Android.UI;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace MarioKart.Android
{
    [Activity(
        Label = "@string/app_name",
        MainLauncher = true,
        ScreenOrientation =
            ScreenOrientation.Landscape)]
    public sealed partial class MainActivity
        : Activity
    {
        //---------------------------------------------------------------------
        // Controles
        //---------------------------------------------------------------------

        private FloatingButton m_btnForward;

        private FloatingButton m_btnReverse;

        private FloatingButton m_btnLeft;

        private FloatingButton m_btnRight;

        private FloatingButton m_btnTurbo;

        private FloatingButton m_btnBluetooth;

        private FloatingButton m_btnSettings;

        private FloatingButton m_btnProfile;

        private TextView m_txtCurrentProfile;

        private DriverCommandSender m_driverCommandSender;

        private TelemetryManager m_telemetryManager;

        private TextView m_txtBluetooth;

        private View m_viewTx;

        //---------------------------------------------------------------------
        // Comunicación
        //---------------------------------------------------------------------

        private BluetoothManager m_bluetoothManager;

        private BluetoothTransport m_transport;

        private CommunicationManager m_communication;

        private BluetoothDiscovery m_discovery;

        private BluetoothConnection m_connection;

        
        //---------------------------------------------------------------------
        // Permisos
        //---------------------------------------------------------------------

        private BluetoothPermissionManager m_permissionManager;

        //---------------------------------------------------------------------
        // Lógica
        //---------------------------------------------------------------------

        private DrivingController m_controller;
        private DriverCommand m_lastSentCommand;
        private readonly List<string> m_console = new List<string>();
        private bool m_developerMode;
        private LinearLayout m_layoutDeveloper;

        //private TextView m_txtDeveloperInfo;
        private TextView m_txtTelemetry;
        private TextView m_txtRSSI;
        private int m_txPackets;

        private int m_rxPackets;

        private DateTime m_lastRx;

        private TextView m_txtProfileTitle;
        private int m_currentRssi = 0;

        private string m_currentProfile = "Sin asignar";

        //---------------------------------------------------------------------
        // Activity
        //---------------------------------------------------------------------

        protected override void OnCreate(
            Bundle savedInstanceState)
        {

            base.OnCreate(savedInstanceState);

            SetContentView(
                Resource.Layout.activity_main);

            InitializeViews();

            InitializeCommunication();

            InitializeController();

            InitializeTelemetry();

            InitializeFloatingButtons();

            UpdateBluetoothState(false);

            if (m_txtCurrentProfile != null)
            {
                m_txtCurrentProfile.Text =
                    "Sin asignar";
            }
        }

        //---------------------------------------------------------------------
        // Inicialización
        //---------------------------------------------------------------------

        private void InitializeViews()
        {
            m_btnForward =
                FindViewById<FloatingButton>(
                    Resource.Id.btnForward);

            m_btnReverse =
                FindViewById<FloatingButton>(
                    Resource.Id.btnReverse);

            m_btnLeft =
                FindViewById<FloatingButton>(
                    Resource.Id.btnLeft);

            m_btnRight =
                FindViewById<FloatingButton>(
                    Resource.Id.btnRight);

            m_btnTurbo =
                FindViewById<FloatingButton>(
                    Resource.Id.btnTurbo);

            m_btnBluetooth =
                FindViewById<FloatingButton>(
                    Resource.Id.btnBluetooth);

            m_btnSettings =
                FindViewById<FloatingButton>(
                    Resource.Id.btnSettings);

            m_viewTx =
                FindViewById<View>(
                Resource.Id.viewTx);

            m_txtBluetooth =
                FindViewById<TextView>(
                    Resource.Id.txtBluetooth);
            m_btnProfile =
                FindViewById<FloatingButton>(
                    Resource.Id.btnProfile);

            m_txtCurrentProfile =
                FindViewById<TextView>(
                    Resource.Id.txtCurrentProfile);

            m_layoutDeveloper =
                FindViewById<LinearLayout>(
                    Resource.Id.layoutDeveloper);

            m_txtTelemetry =
                FindViewById<TextView>(
                    Resource.Id.txtTelemetry);
            m_txtRSSI =
                FindViewById<TextView>(
                    Resource.Id.txtRSSI);


            m_txtProfileTitle =
                FindViewById<TextView>(
                    Resource.Id.txtProfileTitle);
        }
        //---------------------------------------------------------------------

        private async Task AnimateTxAsync()
        {
            if (m_viewTx == null)
            {
                return;
            }

            m_viewTx.ScaleX = 0.8f;
            m_viewTx.ScaleY = 0.8f;
            m_viewTx.Alpha = 0.0f;

            m_viewTx
                .Animate()
                .Alpha(1.0f)
                .ScaleX(1.3f)
                .ScaleY(1.3f)
                .SetDuration(80)
                .Start();

            await Task.Delay(80);

            m_viewTx
                .Animate()
                .Alpha(0.0f)
                .ScaleX(0.8f)
                .ScaleY(0.8f)
                .SetDuration(180)
                .Start();
        }
        private void InitializeCommunication()
        {
            //-------------------------------------------------------------
            // Permisos
            //-------------------------------------------------------------

            m_permissionManager =
                new BluetoothPermissionManager(this);

            //-------------------------------------------------------------
            // Bluetooth
            //-------------------------------------------------------------

            m_connection =
                new BluetoothConnection();

            m_transport =
                new BluetoothTransport(
                    m_connection);

            m_bluetoothManager =
                new BluetoothManager(
                    m_transport);

            //-------------------------------------------------------------
            // Communication
            //-------------------------------------------------------------

            m_communication =
                new CommunicationManager();

            //-------------------------------------------------------------
            // DriverCommand
            //-------------------------------------------------------------

            m_driverCommandSender =
                new DriverCommandSender(
                    m_communication);

            //-------------------------------------------------------------
            // Discovery
            //-------------------------------------------------------------

            m_discovery =
                new BluetoothDiscovery();

            //-------------------------------------------------------------
            // Telemetry
            //-------------------------------------------------------------

            m_telemetryManager =
                m_communication.TelemetryManager;
        }
        //---------------------------------------------------------------------

        //-------------------------------------------------------------
        // Telemetry
        //-------------------------------------------------------------
        private void InitializeTelemetry()
        {
            ConsoleLogger.Log("InitializeTelemetry()");
            ConsoleLogger.Log(
                m_telemetryManager == null
                    ? "Telemetry NULL"
                    : "Telemetry OK");
            if (m_telemetryManager == null)
            {
                return;
            }            
            m_telemetryManager.VehicleStatusReceived +=  OnVehicleStatusReceived;
            ConsoleLogger.Log("Subscribed VehicleStatusReceived");
        }

        private void InitializeController()
        {
            m_controller =
                new DrivingController(
                    m_communication);
        }
        //---------------------------------------------------------------------

        private void InitializeFloatingButtons()
        {
            //----------------------------------------------------------
            // Dirección
            //----------------------------------------------------------

            m_btnForward.Style = FloatingButtonStyles.Primary;

            m_btnForward.SetIcon(Resource.Drawable.ic_forward);

            //----------------------------------------------------------

            m_btnReverse.Style = FloatingButtonStyles.Primary;

            m_btnReverse.SetIcon(Resource.Drawable.ic_reverse);

            //----------------------------------------------------------

            m_btnLeft.Style = FloatingButtonStyles.Primary;

            m_btnLeft.SetIcon(Resource.Drawable.ic_left);

            //----------------------------------------------------------

            m_btnRight.Style = FloatingButtonStyles.Primary;

            m_btnRight.SetIcon(Resource.Drawable.ic_right);

            //----------------------------------------------------------
            // Turbo
            //----------------------------------------------------------

            m_btnTurbo.Style = FloatingButtonStyles.Turbo;

            m_btnTurbo.SetIcon(Resource.Drawable.ic_mushroom);

            //----------------------------------------------------------
            // Bluetooth
            //----------------------------------------------------------

            m_btnBluetooth.Style =FloatingButtonStyles.Bluetooth;

            m_btnBluetooth.SetIcon(Resource.Drawable.ic_bluetooth);

            //----------------------------------------------------------
            // Configuración
            //----------------------------------------------------------

            m_btnSettings.Style =FloatingButtonStyles.Settings;

            m_btnSettings.SetIcon(Resource.Drawable.ic_settings);
            //----------------------------------------------------------
            // Profile
            //----------------------------------------------------------

            m_btnProfile.Style = FloatingButtonStyles.Profile;

            m_btnProfile.SetIcon(Resource.Drawable.ic_profile);

            //----------------------------------------------------------
            // Registrar eventos
            //----------------------------------------------------------

            RegisterDrivingButtons();

            RegisterBluetoothButton();

            RegisterProfileButton();
        }
        //---------------------------------------------------------------------
        // Bluetooth
        //---------------------------------------------------------------------

        private void RegisterBluetoothButton()
        {
            m_btnBluetooth.Button.Click += async (sender, e) =>
            {
                ToggleDeveloperMode();
                await ShowBluetoothDialogAsync();
            };
        }

        private void RegisterProfileButton()
        {
            m_btnProfile.Button.Click += async (sender, e) =>
            {
                await SendProfileChangeAsync();
            };
        }
        //---------------------------------------------------------------------

        //private async Task ShowBluetoothDialogAsync()
        //{
        //    BluetoothDeviceDialog dialog =
        //        new BluetoothDeviceDialog(
        //            this,
        //            m_discovery);

        //    BluetoothDeviceInfo device =
        //        await dialog.SelectDeviceAsync();

        //    if (device == null)
        //    {
        //        return;
        //    }

        //    await ConnectBluetoothAsync(device);
        //}

        private async Task ShowBluetoothDialogAsync()
        {
            //---------------------------------------------------------
            // Permisos
            //---------------------------------------------------------
            ConsoleLogger.Log("RequestPermissionsAsync()");
            //ShowMessage($"SDK: {(int)Build.VERSION.SdkInt}");
            bool granted =
                await m_permissionManager
                    .RequestPermissionsAsync();

            if (!granted)
            {
                ShowMessage(
                    "Bluetooth permission denied.");

                return;
            }

            //---------------------------------------------------------
            // Continuar...
            //---------------------------------------------------------

            var dialog =
                new BluetoothDeviceDialog(
                    this,
                    m_discovery);

            BluetoothDeviceInfo device =
                await dialog.SelectDeviceAsync();

            if (device == null)
            {
                return;
            }

            await ConnectBluetoothAsync(device);
        }
        //---------------------------------------------------------------------

       private async Task SendProfileChangeAsync()
{
    if (!m_communication.IsConnected)
    {
        return;
    }

    await AnimateProfileButtonAsync();

    DriverCommand command =
        m_controller.CurrentCommand;

    //---------------------------------------------------------
    // Enviar PRESIÓN
    //---------------------------------------------------------

    command.DriveMode =
        DriveMode.Gravity;

    await m_driverCommandSender.SendAsync(command);

    m_txPackets++;

    UpdateDeveloperPanel();

    await AnimateTxAsync();

    //---------------------------------------------------------
    // Mantener pulsado un instante
    //---------------------------------------------------------

    await Task.Delay(100);

    //---------------------------------------------------------
    // Enviar LIBERACIÓN
    //---------------------------------------------------------

    command.DriveMode =
        DriveMode.Normal;

    await m_driverCommandSender.SendAsync(command);

    m_txPackets++;

    UpdateDeveloperPanel();

    await AnimateTxAsync();

    StoreLastCommand(command);
}
        private async Task ConnectBluetoothAsync(
            BluetoothDeviceInfo device)
        {
            UpdateBluetoothStateConnecting();

            bool connected =
                await m_communication
                    .ConnectAsync(
                        device.Device);

            UpdateBluetoothState(connected);

            if (connected)
            {
                Toast.MakeText(
                    this,
                    "Conectado a " + device.Name,
                    ToastLength.Short)
                    .Show();
                //---------------------------------------------------------
                // Solicitar sincronización inicial
                //---------------------------------------------------------

                await m_controller.SendCurrentStateAsync();
            }
            else
            {
                Toast.MakeText(
                    this,
                    "No fue posible conectar.",
                    ToastLength.Short)
                    .Show();
            }
        }
        //---------------------------------------------------------------------

        private void UpdateBluetoothState(
            bool connected)
        {
            if (connected)
            {
                m_txtBluetooth.Text =
                    "Connected";

                m_txtBluetooth.SetTextColor(
                        Color.ParseColor(
                        "#4CAF50"));

                m_btnBluetooth.EnableGlow();
            }
            else
            {
                m_txtBluetooth.Text =
                    "Disconnected";

                m_txtBluetooth.SetTextColor(
                    Color.ParseColor(
                        "#F44336"));

                m_btnBluetooth.DisableGlow();
            }
            //-------------------------------------------------------------
            // Driver Profile
            //-------------------------------------------------------------

            if (m_btnProfile != null)
            {
                m_btnProfile.Enabled =
                    connected;

                m_btnProfile.Alpha =
                    connected
                        ? 1.0f
                        : 0.40f;
            }
        }
        //---------------------------------------------------------------------

        private void UpdateBluetoothStateConnecting()
        {
            m_txtBluetooth.Text =
                "Connecting...";

            m_txtBluetooth.SetTextColor(
                Color.ParseColor(
                    "#FFC107"));

            m_btnBluetooth.EnableGlow();

            m_btnBluetooth.SetGlowIntensity(
                180);
        }
        //---------------------------------------------------------------------

        private void RegisterSettingsButton()
        {
            m_btnSettings.Button.Click +=
                (sender, e) =>
                {
                    Toast.MakeText(
                        this,
                        "Configuración próximamente.",
                        ToastLength.Short)
                        .Show();
                };
        }
        //=====================================================================
        // Driving
        //=====================================================================

        /// <summary>
        /// Registra todos los botones de conducción.
        /// </summary>
        private void RegisterDrivingButtons()
        {
            //-------------------------------------------------------------
            // Adelante
            //-------------------------------------------------------------

            RegisterDrivingButton(

                m_btnForward,

                onPressed: () =>
                {
                    m_controller.State.Direction =
                        Direction.Forward;
                },

                onReleased: () =>
                {
                    m_controller.State.Direction =
                        Direction.Stop;
                });

            //-------------------------------------------------------------
            // Atrás
            //-------------------------------------------------------------

            RegisterDrivingButton(

                m_btnReverse,

                onPressed: () =>
                {
                    m_controller.State.Direction =
                        Direction.Reverse;
                },

                onReleased: () =>
                {
                    m_controller.State.Direction =
                        Direction.Stop;
                });

            //-------------------------------------------------------------
            // Izquierda
            //-------------------------------------------------------------

            RegisterDrivingButton(

                m_btnLeft,

                onPressed: () =>
                {
                    m_controller.State.Steering =
                        Steering.Left;
                },

                onReleased: () =>
                {
                    m_controller.State.Steering =
                        Steering.Center;
                });

            //-------------------------------------------------------------
            // Derecha
            //-------------------------------------------------------------

            RegisterDrivingButton(

                m_btnRight,

                onPressed: () =>
                {
                    m_controller.State.Steering =
                        Steering.Right;
                },

                onReleased: () =>
                {
                    m_controller.State.Steering =
                        Steering.Center;
                });

            //-------------------------------------------------------------
            // Turbo
            //-------------------------------------------------------------

            RegisterDrivingButton(

                m_btnTurbo,

                onPressed: () =>
                {
                    m_controller.State.Turbo =
                        Turbo.Enabled;
                },

                onReleased: () =>
                {
                    m_controller.State.Turbo =
                        Turbo.Disabled;
                });
        }

        //---------------------------------------------------------------------

        /// <summary>
        /// Registra un botón de conducción reutilizable.
        /// </summary>
        private void RegisterDrivingButton(

            FloatingButton button,

            Action onPressed,

            Action onReleased)
        {
            if (button == null)
            {
                return;
            }

            button.Button.Touch +=
                async (sender, e) =>
                {
                    switch (e.Event.Action)
                    {
                        case global::Android.Views.MotionEventActions.Down:

                            onPressed?.Invoke();

                            await SendCurrentStateAsync();

                            break;

                        case global::Android.Views.MotionEventActions.Up:

                        case global::Android.Views.MotionEventActions.Cancel:

                            onReleased?.Invoke();

                            await SendCurrentStateAsync();

                            break;
                    }

                    e.Handled = false;
                };
        }

        //---------------------------------------------------------------------

        /// <summary>
        /// Envía el estado actual del vehículo.
        /// </summary>
        private async Task SendCurrentStateAsync()
        {
            if (!m_communication.IsConnected)
            {
                return;
            }

            await m_controller.SendCurrentStateAsync();
            AddConsoleMessage("TX DriverCommand");
        }

        //=====================================================================
        // Utilidades
        //=====================================================================

        /// <summary>
        /// Muestra un mensaje corto al usuario.
        /// </summary>
        private void ShowMessage(
            string message)
        {
            if (string.IsNullOrWhiteSpace(message))
            {
                return;
            }

            Toast.MakeText(
                this,
                message,
                ToastLength.Short)
                .Show();
        }

        //=====================================================================
        // Request Permissions
        //=====================================================================
        public override void OnRequestPermissionsResult(

    int requestCode,

    string[] permissions,

    Permission[] grantResults)
        {
            ConsoleLogger.Log("Permissions callback!");
            base.OnRequestPermissionsResult(

                requestCode,

                permissions,

                grantResults);

            m_permissionManager
                .CompletePermissionRequest(

                    requestCode,

                    grantResults);
        }
        private async Task AnimateProfileAsync(
     string profile)
        {
            if (m_txtCurrentProfile == null)
            {
                return;
            }

            //-------------------------------------------------------------
            // Fade Out
            //-------------------------------------------------------------

            m_txtCurrentProfile
                .Animate()
                .Alpha(0.0f)
                .SetDuration(120)
                .Start();

            await Task.Delay(120);

            //-------------------------------------------------------------
            // Cambiar texto
            //-------------------------------------------------------------

            m_txtCurrentProfile.Text =
                profile;

            //-------------------------------------------------------------
            // Fade In
            //-------------------------------------------------------------

            m_txtCurrentProfile
                .Animate()
                .Alpha(1.0f)
                .SetDuration(120)
                .Start();

            await Task.Delay(120);
        }

        //=====================================================================
        // Ciclo de vida
        //=====================================================================
        private async Task AnimateProfileButtonAsync()
        {
            if (m_btnProfile == null)
            {
                return;
            }

            m_btnProfile.Enabled = false;

            m_btnProfile
                .Animate()
                .RotationBy(360f)
                .SetDuration(350)
                .Start();

            await Task.Delay(350);
        }
        protected override  void OnDestroy()
        {
            //-------------------------------------------------------------
            // Liberar conexión Bluetooth
            //-------------------------------------------------------------

            if (m_communication != null)
            {
                m_communication.Disconnect();
            }

            base.OnDestroy();
        }
        private void VibrateFeedback()
        {
            try
            {
                Vibrator vibrator =
                    (Vibrator)GetSystemService(
                        VibratorService);

                if (vibrator == null)
                {
                    return;
                }

                if (Build.VERSION.SdkInt >= BuildVersionCodes.O)
                {
                    vibrator.Vibrate(
                        VibrationEffect.CreateOneShot(
                            40,
                            VibrationEffect.DefaultAmplitude));
                }
                else
                {
#pragma warning disable CS0618
                    vibrator.Vibrate(40);
#pragma warning restore CS0618
                }
            }
            catch
            {
                // Nunca bloquear la UI por la vibración
            }
        }

        private bool HasCommandChanged(
    DriverCommand current)
        {
            if (m_lastSentCommand == null)
            {
                return true;
            }

            return
                current.Direction != m_lastSentCommand.Direction ||
                current.Steering != m_lastSentCommand.Steering ||
                current.Turbo != m_lastSentCommand.Turbo ||
                current.DriveMode != m_lastSentCommand.DriveMode;
        }
        private void StoreLastCommand(
            DriverCommand command)
        {
            m_lastSentCommand =
                new DriverCommand
                {
                    Direction = command.Direction,
                    Steering = command.Steering,
                    Turbo = command.Turbo,
                    DriveMode = command.DriveMode
                };
        }

        private void AddConsoleMessage(
    string message)
        {
            string line =
                $"{DateTime.Now:HH:mm:ss}  {message}";

            m_console.Add(line);

            //-------------------------------------------------------------
            // Mantener últimos 30 mensajes
            //-------------------------------------------------------------

            while (m_console.Count > 30)
            {
                m_console.RemoveAt(0);
            }

            ConsoleLogger.Log(line);
        }
        private void ToggleDeveloperMode()
        {
            m_developerMode =
                !m_developerMode;

            if (m_layoutDeveloper != null)
            {
                m_layoutDeveloper.Visibility =
                    m_developerMode
                        ? ViewStates.Visible
                        : ViewStates.Gone;
            }

            Toast.MakeText(
                this,
                m_developerMode
                    ? "Developer Mode ON"
                    : "Developer Mode OFF",
                ToastLength.Short)
                .Show();
        }
        private void UpdateDeveloperPanel()
        {
            if (!m_developerMode)
            {
                return;
            }

            //---------------------------------------------------------
            // Perfil
            //---------------------------------------------------------

            if (m_txtCurrentProfile != null)
            {
                m_txtCurrentProfile.Text =
                    m_currentProfile;
            }

            //---------------------------------------------------------
            // Telemetría
            //---------------------------------------------------------

            if (m_txtTelemetry != null)
            {
                m_txtTelemetry.Text =
                    $"TX:{m_txPackets}    RX:{m_rxPackets}";
            }

            //---------------------------------------------------------
            // RSSI
            //---------------------------------------------------------

            if (m_txtRSSI != null)
            {
                m_txtRSSI.Text =
                    $"RSSI: {m_currentRssi} dBm";
            }
        }

        private static string GetProfileName(DrivingProfile profile)
        {
            return profile switch
            {
                DrivingProfile.Rookie => "ROOKIE",
                DrivingProfile.Normal => "NORMAL",
                DrivingProfile.Advanced => "ADVANCED",
                DrivingProfile.Drift => "DRIFT",
                _ => "UNASSIGNED"
            };
        }
        private void OnVehicleStatusReceived(object sender, TelemetryEventArgs e)
        {

            RunOnUiThread(async () =>
            {
                //---------------------------------------------------------
                // Estadísticas
                //---------------------------------------------------------

                m_rxPackets++;

                m_lastRx =
                    DateTime.Now;

                //UpdateDeveloperPanel();

                //---------------------------------------------------------
                // Vibración
                //---------------------------------------------------------

                VibrateFeedback();

                //---------------------------------------------------------
                // Animación del perfil
                //---------------------------------------------------------
                m_currentProfile = GetProfileName(e.VehicleStatus.DrivingProfile);

                //m_currentProfile = 
                //    e.VehicleStatus
                //        .DrivingProfile
                //        .ToString()
                //        .ToUpperInvariant();

                await AnimateProfileAsync(
                    m_currentProfile);

                UpdateDeveloperPanel();

                //---------------------------------------------------------
                // Habilitar botón
                //---------------------------------------------------------

                if (m_btnProfile != null)
                {
                    m_btnProfile.Enabled = true;
                    m_btnProfile.Alpha = 1.0f;
                }
            });
        }
    }
}