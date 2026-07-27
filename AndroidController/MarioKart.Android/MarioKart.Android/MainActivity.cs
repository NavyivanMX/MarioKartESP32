using Android.App;
using Android.OS;
using Android.Views;
using Android.Widget;

using AndroidX.AppCompat.App;

using MarioKart.Android.Communication;
using MarioKart.Android.Communication.Bluetooth;
using MarioKart.Android.Controllers;
using MarioKart.Android.Debug;

namespace MarioKart.Android
{
    [Activity(        Label = "@string/app_name",        Theme = "@style/AppTheme",        MainLauncher = true)]
    public class MainActivity : AppCompatActivity
    {
        //---------------------------------------------------------------------
        // Comunicación
        //---------------------------------------------------------------------

        private readonly BluetoothManager bluetoothManager;

        private readonly BluetoothDiscovery bluetoothDiscovery;

        private readonly BluetoothTransport bluetoothTransport;

        private readonly CommunicationManager communicationManager;

        private readonly DrivingController drivingController;

        //---------------------------------------------------------------------
        // Controles
        //---------------------------------------------------------------------

        private Button btnForward;

        private Button btnReverse;

        private Button btnLeft;

        private Button btnRight;

        private Button btnTurbo;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------

        public MainActivity()
        {
            bluetoothManager =
                new BluetoothManager();

            bluetoothDiscovery =
                new BluetoothDiscovery(
                    bluetoothManager);

            bluetoothTransport =
                new BluetoothTransport(
                    bluetoothManager);

            communicationManager =
                new CommunicationManager(
                    bluetoothTransport);

            drivingController =
                new DrivingController(
                    communicationManager);
        }

        //---------------------------------------------------------------------
        // Ciclo de vida
        //---------------------------------------------------------------------

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            Xamarin.Essentials.Platform.Init(
                this,
                savedInstanceState);

            SetContentView(Resource.Layout.activity_main);

            //-------------------------------------------------------------
            // Buscar controles
            //-------------------------------------------------------------

            btnForward = FindViewById<Button>(Resource.Id.btnForward);

            btnReverse = FindViewById<Button>(Resource.Id.btnReverse);

            btnLeft = FindViewById<Button>(Resource.Id.btnLeft);

            btnRight = FindViewById<Button>(Resource.Id.btnRight);

            btnTurbo = FindViewById<Button>(Resource.Id.btnTurbo);

            //-------------------------------------------------------------
            // Registrar botones
            //-------------------------------------------------------------

            RegisterButton(
                btnForward,
                () => drivingController.State.Forward = true,
                () => drivingController.State.Forward = false);

            RegisterButton(
                btnReverse,
                () => drivingController.State.Reverse = true,
                () => drivingController.State.Reverse = false);

            RegisterButton(
                btnLeft,
                () => drivingController.State.Left = true,
                () => drivingController.State.Left = false);

            RegisterButton(
                btnRight,
                () => drivingController.State.Right = true,
                () => drivingController.State.Right = false);

            RegisterButton(
                btnTurbo,
                () => drivingController.State.Turbo = true,
                () => drivingController.State.Turbo = false);

            //-------------------------------------------------------------
            // Mostrar dispositivos encontrados (temporal)
            //-------------------------------------------------------------

            foreach (var device in bluetoothDiscovery.GetPairedDevices())
            {
                ConsoleLogger.Log(device.ToString());
            }
        }

        //---------------------------------------------------------------------
        // Utilidades
        //---------------------------------------------------------------------

        private void RegisterButton(
            Button button,
            System.Action pressed,
            System.Action released)
        {
            button.Touch += async (sender, e) =>
            {
                switch (e.Event.Action)
                {
                    case MotionEventActions.Down:

                        pressed();

                        await drivingController.UpdateAsync();

                        break;

                    case MotionEventActions.Up:

                        released();

                        await drivingController.UpdateAsync();

                        break;
                }

                e.Handled = true;
            };
        }
    }
}