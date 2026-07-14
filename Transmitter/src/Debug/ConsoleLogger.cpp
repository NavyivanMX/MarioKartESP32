/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ConsoleLogger.cpp
 ******************************************************************************/

#include "src/Debug/ConsoleLogger.h"

#include <Arduino.h>

// #include "src/Shared/DriverCommandFormatter.h"
// #include "src/Shared/Version.h"

#include <MKShared.h>
#include <Utilities/DriverCommandFormatter.h>
#include <Version/Version.h>

namespace MK
{

//=============================================================================

void ConsoleLogger::Begin()
{
    Serial.begin(115200);

    while (!Serial)
    {
        delay(10);
    }
}

//=============================================================================

void ConsoleLogger::LogBoot()
{
    Serial.println();
    Serial.println(F("========================================"));
    Serial.println(Version::Project);

    Serial.print(F("Release : "));
    Serial.println(Version::Release);

    Serial.print(F("Build   : "));

    if constexpr (Config::Build::InputTestMode)
    {
        Serial.println(F("INPUT TEST"));
    }
    else
    {
        Serial.println(F("NORMAL"));
    }

    Serial.println(F("========================================"));
    Serial.println();
}

//=============================================================================

void ConsoleLogger::LogReady()
{
    Serial.print(F("[READY] "));

    if constexpr (Config::Build::InputTestMode)
    {
        Serial.println(F("Input test mode."));
    }
    else
    {
        Serial.println(F("Controller ready."));
    }
}

//=============================================================================

void ConsoleLogger::LogError(const char* message)
{
    Serial.print(F("[ERROR] "));
    Serial.println(message);
}

//=============================================================================

void ConsoleLogger::Log(const Protocol::DriverCommand& command)
{
    //----------------------------------------------------------
    // Registrar únicamente cambios.
    //----------------------------------------------------------

    if (command == m_previousCommand)
    {
        return;
    }

    m_previousCommand = command;

    LogCommand(command);
}

//=============================================================================

void ConsoleLogger::LogCommand(const Protocol::DriverCommand& command)
{
    Serial.print(F("[INPUT] "));
    Serial.println(Protocol::DriverCommandFormatter::Format(command));
}

} // namespace MK