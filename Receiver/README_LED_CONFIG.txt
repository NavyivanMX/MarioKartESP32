MarioKart ESP32 RC - LED Count Configuration

Default values:
  AmbientLight = 6 LEDs, GPIO 19
  RearLight    = 2 LEDs, GPIO 4

Serial commands:
  MKCFG GET
  MKCFG SET AMBIENT_LEDS=8
  MKCFG SET REAR_LEDS=4
  MKCFG CLEAR

After SET/CLEAR:
  MKCFG RESTART_REQUIRED

The new values are stored in NVS and become active after restarting
the Receiver.

No installer changes are included in this package yet.
Test the Receiver firmware first.
