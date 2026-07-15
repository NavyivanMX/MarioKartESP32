/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ConsoleLogger.h
 *
 * Descripción:
 * Módulo de depuración mediante el puerto serie.
 *
 * Responsabilidades:
 *  - Inicializar Serial.
 *  - Mostrar información de inicio.
 *  - Mostrar errores.
 *  - Registrar cambios en DriverCommand.
 ******************************************************************************/

#include "src/Debug/ConsoleLogger.h"
#include <Arduino.h>
#include <Utilities/DriverCommandFormatter.h>
#include "src/Config/TransmitterConfig.h"
#include "src/Firmware/Firmware.h"
namespace MK
{
//
//=============================================================================
// Inicialización
//
//=============================================================================
void ConsoleLogger::Begin()
{
Serial.begin(115200);
while (!Serial)
{
delay(10);
}
}
//
//=============================================================================
// Información de arranque
//
//=============================================================================
void ConsoleLogger::LogBoot()
{
Serial.println();
Serial.println(F("========================================"));
Serial.println(Firmware::ProjectName);
Serial.println(Firmware::FirmwareName);
Serial.print(F("Version : "));

Serial.println(Firmware::VersionString);
Serial.print(F("Release : "));
Serial.println(Firmware::Release);
Serial.print(F("Author : "));
Serial.println(Firmware::Author);
if constexpr (TransmitterConfig::InputTestMode)
{
Serial.println(F("Input Test Mode"));
}
else
{
Serial.println(F("ESP-NOW Mode"));
}
Serial.println(F("========================================"));
Serial.println();
}
//
//=============================================================================
// Sistema listo
//
//=============================================================================
void ConsoleLogger::LogReady()
{
Serial.println(F("----------------------------------------"));
Serial.println(F("System Ready"));
Serial.println(F("----------------------------------------"));
}
//
//=============================================================================
// Errores
//
//=============================================================================
void ConsoleLogger::LogError(const char* message)
{
Serial.print(F("[ERROR] "));
Serial.println(message);
}
//
//=============================================================================
// Registro de comandos
//
//=============================================================================
void ConsoleLogger::Log(const DriverCommand& command)
{
// Registrar únicamente cuando exista un cambio.
if (command == m_previousCommand)
{
return;
}
m_previousCommand = command;
LogCommand(command);
}
//-----------------------------------------------------------------------------
void ConsoleLogger::LogCommand(const DriverCommand& command)
{
Serial.print(F("[INPUT] "));
Serial.println(DriverCommandFormatter::Format(command));
}
} // namespace MK
