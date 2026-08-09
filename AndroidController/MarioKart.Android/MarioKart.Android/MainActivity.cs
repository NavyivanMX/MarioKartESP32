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
using System.Threading;
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

        private CancellationTokenSource m_driverLoopCancellation;

        //---------------------------------------------------------------------
        // Comunicación
        //---------------------------------------------------------------------

        //private BluetoothManager m_bluetoothManager;

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
        private bool m_developerPanelVisible = false;

        private string m_currentProfile = "Sin asignar";
        private View m_profileIndicator;

        private CancellationTokenSource m_turboVibrationCancellation;
        private Task m_turboVibrationTask;

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

            UpdateBluetoothState(m_communication.IsConnected);

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

   

            m_txtBluetooth =
                FindViewById<TextView>(
                    Resource.Id.txtBluetooth);
            m_btnProfile =
                FindViewById<FloatingButton>(
                    Resource.Id.btnProfile);

            m_btnProfile.LongClick +=
                OnProfileLongClick;

            m_txtCurrentProfile =
                FindViewById<TextView>(
                    Resource.Id.txtProfile);

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

            m_profileIndicator =
                FindViewById<View>(
                    Resource.Id.viewProfileIndicator);

       //     m_viewTx =
       //FindViewById<View>(
       //Resource.Id.viewTx);
        }
        //---------------------------------------------------------------------
        private void UpdateProfileDisplay(
    DrivingProfile profile)
        {
            switch (profile)
            {
                case DrivingProfile.Rookie:

                    m_txtCurrentProfile.Text =
                        "ROOKIE";

                    m_profileIndicator.SetBackgroundResource(
                        Resource.Drawable.circle_green);

                    break;

                case DrivingProfile.Normal:

                    m_txtCurrentProfile.Text =
                        "NORMAL";

                    m_profileIndicator.SetBackgroundResource(
                        Resource.Drawable.circle_blue);

                    break;

                case DrivingProfile.Advanced:

                    m_txtCurrentProfile.Text =
                        "ADVANCED";

                    m_profileIndicator.SetBackgroundResource(
                        Resource.Drawable.circle_orange);

                    break;

                case DrivingProfile.Drift:

                    m_txtCurrentProfile.Text =
                        "DRIFT";

                    m_profileIndicator.SetBackgroundResource(
                        Resource.Drawable.circle_purple);

                    break;
            }
        }
        private void UpdateProfileIndicator(
    DrivingProfile profile)
        {
            if (m_profileIndicator == null)
            {
                return;
            }

            switch (profile)
            {
                case DrivingProfile.Rookie:

                    m_profileIndicator.SetBackgroundResource(
                        Resource.Drawable.circle_green);

                    break;

                case DrivingProfile.Normal:

                    m_profileIndicator.SetBackgroundResource(
                        Resource.Drawable.circle_blue);

                    break;

                case DrivingProfile.Advanced:

                    m_profileIndicator.SetBackgroundResource(
                        Resource.Drawable.circle_orange);

                    break;

                case DrivingProfile.Drift:

                    m_profileIndicator.SetBackgroundResource(
                        Resource.Drawable.circle_purple);

                    break;
            }
        }
        private async Task AnimateProfileIndicatorAsync()
        {
            if (m_profileIndicator == null)
            {
                return;
            }

            m_profileIndicator
                .Animate()
                .ScaleX(1.35f)
                .ScaleY(1.35f)
                .SetDuration(120);

            await Task.Delay(120);

            m_profileIndicator
                .Animate()
                .ScaleX(1.0f)
                .ScaleY(1.0f)
                .SetDuration(120);
        }
        private async Task AnimateTxAsync()
        {
            //if (m_viewTx == null)
            //{
            //    return;
            //}

            //m_viewTx.ScaleX = 0.8f;
            //m_viewTx.ScaleY = 0.8f;
            //m_viewTx.Alpha = 0.0f;

            //m_viewTx
            //    .Animate()
            //    .Alpha(1.0f)
            //    .ScaleX(1.3f)
            //    .ScaleY(1.3f)
            //    .SetDuration(80)
            //    .Start();

            //await Task.Delay(80);

            //m_viewTx
            //    .Animate()
            //    .Alpha(0.0f)
            //    .ScaleX(0.8f)
            //    .ScaleY(0.8f)
            //    .SetDuration(180)
            //    .Start();
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

            //m_bluetoothManager =
            //    new BluetoothManager(
            //        m_transport);

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
            if (m_telemetryManager == null)
            {
                return;
            }            
            m_telemetryManager.VehicleStatusReceived +=  OnVehicleStatusReceived;
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

            RegisterSettingsButton();
        }
        //---------------------------------------------------------------------
        // Bluetooth
        //---------------------------------------------------------------------

        private void RegisterBluetoothButton()
        {            
            m_btnBluetooth.Click +=
                async (s, e) =>
                {         
                    //---------------------------------------------------------
                    // Ya conectado
                    //---------------------------------------------------------                 
                    if (m_communication.IsConnected)
                    {
                        ShowDisconnectDialog();
                        return;
                    }

                    //---------------------------------------------------------
                    // Conectar
                    //---------------------------------------------------------

                    await ShowBluetoothDialogAsync();
                };
        }
        private void ShowDisconnectDialog()
        {
            new AlertDialog.Builder(this)

                .SetTitle("Bluetooth")

                .SetMessage(
                    "Actualmente estás conectado a Mario Kart RC.\n\n¿Deseas desconectarte?")

                .SetNegativeButton(
                    "Cancelar",
                    (s, e) => { })

                .SetPositiveButton(
                    "Desconectar",
                    (s, e) =>
                    {
                        DisconnectBluetooth();
                    })

                .Show();
        }

        public void Disconnect()
        {
            m_connection.Disconnect();
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

                m_driverLoopCancellation = new CancellationTokenSource();

                _ = DriverLoopAsync( m_driverLoopCancellation.Token);
            }
            else
            {
                Toast.MakeText(
                    this,
                    "No fue posible conectar.",
                    ToastLength.Short)
                    .Show();
                m_driverLoopCancellation?.Cancel();
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
                m_driverLoopCancellation?.Cancel();
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
                    ToggleDeveloperPanel();
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

                    StartTurboVibration();

                    _ = AnimateTurboAsync();
                },

                onReleased: () =>
                {
                    m_controller.State.Turbo =
                        Turbo.Disabled;

                    StopTurboVibration();
                }
                );
        }
        private void DisconnectBluetooth()
        {
            //---------------------------------------------------------
            // Desconectar Bluetooth
            //---------------------------------------------------------

            m_communication.Disconnect();

            //---------------------------------------------------------
            // Estado
            //---------------------------------------------------------

            m_currentProfile =
                "NOT CONNECTED";

            //---------------------------------------------------------
            // Panel Developer
            //---------------------------------------------------------

            UpdateDeveloperPanel();
            UpdateBluetoothState(m_communication.IsConnected);

            //---------------------------------------------------------
            // Botón Bluetooth
            //---------------------------------------------------------

            if (m_btnBluetooth != null)
            {
                m_btnBluetooth.Selected = false;
            }

            //---------------------------------------------------------
            // Botón Profile
            //---------------------------------------------------------

            if (m_btnProfile != null)
            {
                m_btnProfile.Enabled = false;
                m_btnProfile.Alpha = 0.40f;
            }

            //---------------------------------------------------------
            // Indicador
            //---------------------------------------------------------

            if (m_profileIndicator != null)
            {
                m_profileIndicator.Visibility =
                    ViewStates.Invisible;
            }
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

                            break;

                        case global::Android.Views.MotionEventActions.Up:

                        case global::Android.Views.MotionEventActions.Cancel:

                            onReleased?.Invoke();

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
            base.OnRequestPermissionsResult(

                requestCode,

                permissions,

                grantResults);

            m_permissionManager
                .CompletePermissionRequest(

                    requestCode,

                    grantResults);
        }
        private async Task AnimateTurboAsync()
{
    if (m_btnTurbo == null)
    {
        return;
    }

    //---------------------------------------------
    // Escala
    //---------------------------------------------

    m_btnTurbo.Animate()
        .ScaleX(1.18f)
        .ScaleY(1.18f)
        .SetDuration(100);

            //---------------------------------------------
            // Color
            //---------------------------------------------

            m_btnTurbo.BackgroundTintList =
                global::Android.Content.Res.ColorStateList.ValueOf(
                    new global::Android.Graphics.Color(
                        GetColor(Resource.Color.turboOrange)));

    //---------------------------------------------

    await Task.Delay(100);

    //---------------------------------------------
    // Regresar
    //---------------------------------------------

    m_btnTurbo.Animate()
        .ScaleX(1.0f)
        .ScaleY(1.0f)
        .SetDuration(120);

    m_btnTurbo.BackgroundTintList =
         global::Android.Content.Res.ColorStateList.ValueOf(
            new global::Android.Graphics.Color(
                GetColor(Resource.Color.turboYellow)));
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
        private void OnProfileLongClick(object sender,View.LongClickEventArgs e)
        {
            ToggleDeveloperPanel();

            e.Handled = true;
        }
        private void ToggleDeveloperPanel()
        {
            if (m_layoutDeveloper == null)
            {
                return;
            }

            m_developerPanelVisible =
                !m_developerPanelVisible;

            m_layoutDeveloper.Visibility =
                m_developerPanelVisible
                    ? ViewStates.Visible
                    : ViewStates.Gone;
        }

        private void VibrateTurbo()
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
                            150,
                            VibrationEffect.DefaultAmplitude));
                }
                else
                {
#pragma warning disable CS0618
                    vibrator.Vibrate(150);
#pragma warning restore CS0618
                }
            }
            catch
            {
            }
        }

        private async Task DriverLoopAsync(
        CancellationToken token)
            {
                while (!token.IsCancellationRequested)
                {
                    await SendCurrentCommandAsync();

                    await Task.Delay(50, token);
                }
            }
        private async Task SendCurrentCommandAsync()
        {
            //-------------------------------------------------------------
            // Sin conexión
            //-------------------------------------------------------------

            if (!m_communication.IsConnected)
            {
                return;
            }

            //-------------------------------------------------------------
            // Obtener comando actual
            //-------------------------------------------------------------

            DriverCommand command =
                m_controller.CurrentCommand;

            //-------------------------------------------------------------
            // Enviar SIEMPRE
            //-------------------------------------------------------------

            await m_driverCommandSender.SendAsync(
                command);

            //-------------------------------------------------------------
            // Estadísticas
            //-------------------------------------------------------------

            m_txPackets++;

            //m_lastTx =
            //    DateTime.Now;

            UpdateDeveloperPanel();
        }

        private void StartTurboVibration()
        {
            if (m_turboVibrationTask != null &&
                !m_turboVibrationTask.IsCompleted)
            {
                return;
            }

            m_turboVibrationCancellation =
                new CancellationTokenSource();

            m_turboVibrationTask =
                TurboVibrationLoopAsync(
                    m_turboVibrationCancellation.Token);
        }
        private async Task TurboVibrationLoopAsync(
    CancellationToken token)
        {
            try
            {
                while (!token.IsCancellationRequested)
                {
                    VibrateTurbo();

                    await Task.Delay(
                        120,
                        token);
                }
            }
            catch (TaskCanceledException)
            {
            }
        }

        private void StopTurboVibration()
        {
            m_turboVibrationCancellation?.Cancel();

            m_turboVibrationCancellation = null;
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
              
                //---------------------------------------------------------
                // Animación del perfil
                //---------------------------------------------------------
                m_currentProfile = GetProfileName(e.VehicleStatus.DrivingProfile);

                //m_currentProfile = 
                //    e.VehicleStatus
                //        .DrivingProfile
                //        .ToString()
                //        .ToUpperInvariant();

                await AnimateProfileAsync(m_currentProfile);

                UpdateDeveloperPanel();
                UpdateProfileIndicator(e.VehicleStatus.DrivingProfile);

                UpdateProfileDisplay(e.VehicleStatus.DrivingProfile);

                await AnimateProfileIndicatorAsync();                

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