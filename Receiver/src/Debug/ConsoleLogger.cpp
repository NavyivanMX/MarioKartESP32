/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ConsoleLogger.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 ******************************************************************************/

#include "src/Debug/ConsoleLogger.h"
#include "src/Vehicle/MotorDirectionToString.h"
#include <Protocol/DriverCommandFormatter.h>

namespace MK
{

    ConsoleLogger& ConsoleLogger::Instance() noexcept
{
    static ConsoleLogger instance;

    return instance;
}

void ConsoleLogger::Initialize() noexcept
{
    Serial.begin(115200);

    while (!Serial)
    {
    }
}

void ConsoleLogger::LogBoot() noexcept
{
    Serial.println();
    Serial.println("========================================");
    Serial.println("        MarioKart ESP32 Receiver");
    Serial.println("========================================");
    Serial.println();

    Serial.print("Version : ");
    Serial.println(Version::Project);

    Serial.println();
}

void ConsoleLogger::LogCommand(
    const Protocol::DriverCommand& command) noexcept
{
    Serial.print("INPUT -> ");

    Serial.println(
        DriverCommandFormatter::Format(command));
}

void ConsoleLogger::LogMotorState(
    const MotorState& left,
    const MotorState& right) noexcept
{
    Serial.println("------------- MOTORS -------------");

    PrintMotor("LEFT ", left);

    PrintMotor("RIGHT", right);

    Serial.println("----------------------------------");
}

void ConsoleLogger::PrintMotor(
    const char* name,
    const MotorState& state) noexcept
{
    Serial.print(name);

    Serial.print(" : ");

    Serial.print(
        ToString(state.direction));

    if (state.turbo == Types::Vehicle::Turbo::On)
    {
        Serial.print(" + TURBO");
    }

    Serial.println();
}

void ConsoleLogger::LogGravity(
    bool enabled) noexcept
{
    Serial.print("Gravity : ");

    Serial.println(
        enabled
            ? "Enabled"
            : "Normal");
}

void ConsoleLogger::LogError(
    const char* message) noexcept
{
    Serial.print("[ERROR] ");

    Serial.println(message);
}

}