/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : GravityDriver.cpp
 ******************************************************************************/

#include "src/Drivers/GravityDriver.h"
#include "src/Config/Pins.h"
#include <Arduino.h>


namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

bool GravityDriver::Begin() noexcept
{
    pinMode(Pins::Gravity, OUTPUT);

    SetMode(Types::Vehicle::DriveMode::Normal);

    return true;
}

//=============================================================================
// Control
//=============================================================================

void GravityDriver::SetMode(
    Types::Vehicle::DriveMode mode) noexcept
{

    digitalWrite(
        Pins::Gravity,
        mode == Types::Vehicle::DriveMode::Gravity
            ? HIGH
            : LOW);
}

} // namespace MK