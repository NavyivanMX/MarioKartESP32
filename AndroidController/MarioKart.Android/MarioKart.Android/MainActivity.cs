using Android.App;
using Android.OS;
using Android.Runtime;
using AndroidX.AppCompat.App;
using MarioKart.Android.Controllers;
using Android.Widget;
using Android.Views;

using MarioKart.Android.Protocol;

namespace MarioKart.Android
{
    [Activity(Label = "@string/app_name", Theme = "@style/AppTheme", MainLauncher = true)]
    public class MainActivity : AppCompatActivity
    {
        private readonly DrivingController m_controller =    new DrivingController();
        private Button m_btnForward;

        private Button m_btnReverse;

        private Button m_btnLeft;

        private Button m_btnRight;

        private Button m_btnTurbo;
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            Xamarin.Essentials.Platform.Init(this, savedInstanceState);
            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.activity_main);

            m_btnForward = FindViewById<Button>(Resource.Id.btnForward);
            m_btnReverse = FindViewById<Button>(Resource.Id.btnReverse);
            m_btnLeft = FindViewById<Button>(Resource.Id.btnLeft);
            m_btnRight = FindViewById<Button>(Resource.Id.btnRight);
            m_btnTurbo = FindViewById<Button>(Resource.Id.btnTurbo);

            
            RegisterButton(m_btnForward, () => m_controller.State.Forward = true, () => m_controller.State.Forward = false);
            RegisterButton(m_btnReverse, () => m_controller.State.Reverse = true, () => m_controller.State.Reverse = false);
            RegisterButton(m_btnLeft, () => m_controller.State.Left = true, () => m_controller.State.Left = false);
            RegisterButton(m_btnRight, () => m_controller.State.Right = true, () => m_controller.State.Right = false);
            RegisterButton(m_btnTurbo, () => m_controller.State.Turbo = true, () => m_controller.State.Turbo = false);
        }

        private void RegisterButton(
    Button button,
    System.Action pressed,
    System.Action released)
        {
            button.Touch += (sender, e) =>
            {
                switch (e.Event.Action)
                {
                    case MotionEventActions.Down:

                        pressed();

                        m_controller.Update();

                        break;

                    case MotionEventActions.Up:

                        released();

                        m_controller.Update();

                        break;
                }

                e.Handled = true;
            };
        }

    }
}