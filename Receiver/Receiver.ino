/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MK.ino
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Punto de entrada del firmware Receiver.
 ******************************************************************************/

#include "src/Core/ReceiverController.h"

#include <MKShared.h>

namespace
{

MK::ReceiverController g_receiverController;

} // namespace

//=============================================================================
// Arduino
//=============================================================================

void setup()

{
    if (!g_receiverController.Begin())
    {
        while (true)
        {
            // Estado de fallo permanente.
            // El sistema requiere reinicio manual.

            delay(1000);
        }
    }
}

void loop()
{
    g_receiverController.Update();    
    delay(MK::Config::System::LoopPeriodMs);
}