#include <MKShared.h>

/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MK.ino
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Punto de entrada del transmisor.
 ******************************************************************************/

#include "src/Core/ReceiverController.h"
#include <Protocol/DriverCommandFormatter.h>

namespace
{

MK::ReceiverController g_receiverController;

} // namespace

void setup()
{
    g_receiverController.Initialize();
}

void loop()
{
    g_receiverController.Update();

    delay(MK::Config::System::LoopPeriodMs);
}