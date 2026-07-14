/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ConsoleLogger.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Servicio de registro por consola.
 * Centraliza toda la salida Serial del sistema.
 ******************************************************************************/

#pragma once

#include <Arduino.h>

#include <MKShared.h>

#include "src/Vehicle/MotorState.h"

namespace MK
{

class ConsoleLogger
{
public:

    static ConsoleLogger& Instance() noexcept;

    void Initialize() noexcept;

    void LogBoot() noexcept;

    void LogCommand(
        const Protocol::DriverCommand& command) noexcept;

    void LogMotorState(
        const MotorState& left,
        const MotorState& right) noexcept;

    void LogGravity(
        bool enabled) noexcept;

    void LogError(
        const char* message) noexcept;

private:

    ConsoleLogger() = default;

    void PrintMotor(
        const char* name,
        const MotorState& state) noexcept;
};

}