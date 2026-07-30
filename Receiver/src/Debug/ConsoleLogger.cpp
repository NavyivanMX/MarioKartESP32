/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ConsoleLogger.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del logger de consola.
 ******************************************************************************/

#include "src/Debug/ConsoleLogger.h"

#include <Arduino.h>
#include <WiFi.h>

namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

void ConsoleLogger::Begin() noexcept
{
    Serial.begin(115200);

    while (!Serial)
    {
    }
}

//=============================================================================
// Sistema
//=============================================================================

void ConsoleLogger::LogBoot() const noexcept
{
    Serial.println();
    Serial.println(F("========================================"));
    Serial.println(F("MarioKart ESP32 RC"));
    Serial.println(F("Receiver"));
    Serial.println(F("Version : 1.1.0"));
    Serial.println(F("Release : RC1.1"));
    Serial.println(F("Author  : Narciso Ivan Cisneros Acosta"));
    Serial.println(F("========================================"));
    Serial.println();

    //---------------------------------------------------------------------
    // Información del sistema
    //---------------------------------------------------------------------

    LogWiFiMac();

    Serial.println();
}

void ConsoleLogger::LogReady() const noexcept
{
    Serial.println(F("----------------------------------------"));
    Serial.println(F("System Ready"));
    Serial.println(F("----------------------------------------"));
}

void ConsoleLogger::LogError(
    const char* message) const noexcept
{
    Serial.print(F("[ERROR] "));
    Serial.println(message);
}

//=============================================================================
// Comunicación
//=============================================================================

void ConsoleLogger::LogWiFiMac() const noexcept
{
    Serial.print(F("WiFi MAC   : "));
    Serial.println(WiFi.macAddress());
}

void ConsoleLogger::LogBluetooth(
    const char* deviceName) const noexcept
{
    Serial.print(F("Bluetooth  : "));
    Serial.println(deviceName);
}

void ConsoleLogger::LogBluetoothConnected() const noexcept
{
    Serial.println(F("[Bluetooth] Client connected."));
}

//=============================================================================
// DriverCommand
//=============================================================================

void ConsoleLogger::Log(
    const Protocol::DriverCommand& command) const noexcept
{
    Serial.println();

    Serial.println(F("========== DriverCommand =========="));

    Serial.print(F("Direction : "));
    Serial.println(ToString(command.direction));

    Serial.print(F("Steering  : "));
    Serial.println(ToString(command.steering));

    Serial.print(F("Turbo     : "));
    Serial.println(ToString(command.turbo));

    Serial.print(F("DriveMode : "));
    Serial.println(ToString(command.driveMode));

    Serial.println(F("==================================="));
}

//=============================================================================
// Conversión de enums
//=============================================================================

const char* ConsoleLogger::ToString(
    Types::Vehicle::Direction direction) noexcept
{
    switch (direction)
    {
        case Types::Vehicle::Direction::Stop:
            return "Stop";

        case Types::Vehicle::Direction::Forward:
            return "Forward";

        case Types::Vehicle::Direction::Reverse:
            return "Reverse";
    }

    return "Unknown";
}

const char* ConsoleLogger::ToString(
    Types::Vehicle::Steering steering) noexcept
{
    switch (steering)
    {
        case Types::Vehicle::Steering::Straight:
            return "Straight";

        case Types::Vehicle::Steering::Left:
            return "Left";

        case Types::Vehicle::Steering::Right:
            return "Right";
    }

    return "Unknown";
}

const char* ConsoleLogger::ToString(
    Types::Vehicle::Turbo turbo) noexcept
{
    switch (turbo)
    {
        case Types::Vehicle::Turbo::Disabled:
            return "Disabled";

        case Types::Vehicle::Turbo::Enabled:
            return "Enabled";
    }

    return "Unknown";
}

const char* ConsoleLogger::ToString(
    Types::Vehicle::DriveMode mode) noexcept
{
    switch (mode)
    {
        case Types::Vehicle::DriveMode::Normal:
            return "Normal";

        case Types::Vehicle::DriveMode::Gravity:
            return "Gravity";
    }

    return "Unknown";
}

} // namespace MK