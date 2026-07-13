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

#ifndef MK_CONSOLELOGGER_H
#define MK_CONSOLELOGGER_H

// #include "src/Shared/Protocol.h"
// #include "src/Shared/Config.h"

#include <MKShared.h>

namespace MK
{

class ConsoleLogger final
{
public:

    ConsoleLogger() = default;

    ~ConsoleLogger() = default;

    void Begin();

    void LogBoot();

    void LogReady();

    void LogError(const char* message);

    void Log(const Protocol::DriverCommand& command);

private:

    void LogCommand(const Protocol::DriverCommand& command);

private:

    Protocol::DriverCommand m_previousCommand{};

};

} // namespace MK

#endif // MK_CONSOLELOGGER_H