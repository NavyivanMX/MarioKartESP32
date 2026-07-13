#include <MKShared.h>

/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MK.ino
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Punto de entrada del transmisor.
 ******************************************************************************/

#include "src/Core/Controller.h"

namespace
{

MK::Controller g_controller;

} // namespace

void setup()
{
    if (!g_controller.Begin())
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
    g_controller.Update();

    delay(MK::Config::System::LoopPeriodMs);
}