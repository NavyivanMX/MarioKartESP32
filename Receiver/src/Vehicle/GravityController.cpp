/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : VehicleController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 ******************************************************************************/

#include "src/Vehicle/GravityController.h"

namespace MK
{

    void GravityController::Enable() noexcept
    {
        if (m_enabled)
            return;

        m_enabled = true;

        Serial.println("Gravity Mode");
    }

    void GravityController::Disable() noexcept
    {
        if (!m_enabled)
            return;

        m_enabled = false;

        Serial.println("Normal Mode");
    }

}