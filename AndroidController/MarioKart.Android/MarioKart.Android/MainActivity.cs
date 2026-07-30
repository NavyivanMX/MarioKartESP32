/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MainActivity.cs
 ******************************************************************************/

using Android.App;
using Android.Content.PM;
using Android.Graphics;
using Android.OS;
using Android.Widget;
using MarioKart.Android.Communication;
using MarioKart.Android.Communication.Bluetooth;
using MarioKart.Android.Controllers;
using MarioKart.Android.Controls;
using MarioKart.Android.Debug;
using MarioKart.Android.Shared;
using MarioKart.Android.UI;
using System;
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

        private TextView m_txtBluetooth;

        //---------------------------------------------------------------------
        // Comunicación
        //---------------------------------------------------------------------

        private BluetoothManager m_bluetoothManager;

        private BluetoothTransport m_transport;

        private CommunicationManager m_communication;

        private BluetoothDiscovery m_discovery;

        //---------------------------------------------------------------------
        // Permisos
        //---------------------------------------------------------------------

        private BluetoothPermissionManager m_permissionManager;

        //---------------------------------------------------------------------
        // Lógica
        //---------------------------------------------------------------------

        private DrivingController m_controller;

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

            InitializeFloatingButtons();

            UpdateBluetoothState(false);
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
        }
        //---------------------------------------------------------------------

        private void InitializeCommunication()
        {
            m_permissionManager =
                new BluetoothPermissionManager(this);

            m_bluetoothManager =
                new BluetoothManager();

            m_transport =
                new BluetoothTransport(
                    m_bluetoothManager);

            m_communication =
                new CommunicationManager(
                    m_transport);

            m_discovery =
                new BluetoothDiscovery(
                    m_bluetoothManager);
        }
        //---------------------------------------------------------------------

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

            m_btnForward.Style =
                FloatingButtonStyles.Primary;

            m_btnForward.SetIcon(
                Resource.Drawable.ic_forward);

            //----------------------------------------------------------

            m_btnReverse.Style =
                FloatingButtonStyles.Primary;

            m_btnReverse.SetIcon(
                Resource.Drawable.ic_reverse);

            //----------------------------------------------------------

            m_btnLeft.Style =
                FloatingButtonStyles.Primary;

            m_btnLeft.SetIcon(
                Resource.Drawable.ic_left);

            //----------------------------------------------------------

            m_btnRight.Style =
                FloatingButtonStyles.Primary;

            m_btnRight.SetIcon(
                Resource.Drawable.ic_right);

            //----------------------------------------------------------
            // Turbo
            //----------------------------------------------------------

            m_btnTurbo.Style =
                FloatingButtonStyles.Turbo;

            m_btnTurbo.SetIcon(
                Resource.Drawable.ic_mushroom);

            //----------------------------------------------------------
            // Bluetooth
            //----------------------------------------------------------

            m_btnBluetooth.Style =
                FloatingButtonStyles.Bluetooth;

            m_btnBluetooth.SetIcon(
                Resource.Drawable.ic_bluetooth);

            //----------------------------------------------------------
            // Configuración
            //----------------------------------------------------------

            m_btnSettings.Style =
                FloatingButtonStyles.Settings;

            m_btnSettings.SetIcon(
                Resource.Drawable.ic_settings);

            //----------------------------------------------------------
            // Registrar eventos
            //----------------------------------------------------------

            RegisterDrivingButtons();

            RegisterBluetoothButton();

            RegisterSettingsButton();
        }
        //---------------------------------------------------------------------
        // Bluetooth
        //---------------------------------------------------------------------

        private void RegisterBluetoothButton()
        {
            m_btnBluetooth.Button.Click += async (sender, e) =>
            {
                await ShowBluetoothDialogAsync();
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

        private async Task ConnectBluetoothAsync(
            BluetoothDeviceInfo device)
        {
            UpdateBluetoothStateConnecting();

            bool connected =
                await m_communication
                    .ConnectBluetoothAsync(
                        device.Device);

            UpdateBluetoothState(connected);

            if (connected)
            {
                Toast.MakeText(
                    this,
                    "Conectado a " + device.Name,
                    ToastLength.Short)
                    .Show();
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

        //=====================================================================
        // Ciclo de vida
        //=====================================================================

        protected override async void OnDestroy()
        {
            //-------------------------------------------------------------
            // Liberar conexión Bluetooth
            //-------------------------------------------------------------

            if (m_communication != null)
            {
                await m_communication.DisconnectAsync();
            }

            base.OnDestroy();
        }
    }
}