/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotorDriver.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del driver de hardware para el sistema de propulsión.
 ******************************************************************************/

#include "src/Drivers/MotorDriver.h"

#include <Arduino.h>

#include "src/Config/MotorConfig.h"
#include "src/Config/Pins.h"

namespace
{

//=============================================================================
// Utilidades
//=============================================================================

constexpr std::uint8_t ToPwm(
    MK::Types::Vehicle::Turbo turbo) noexcept
{
    return (turbo == MK::Types::Vehicle::Turbo::Enabled)
        ? MK::MotorConfig::Pwm::Turbo
        : MK::MotorConfig::Pwm::Normal;
}

} // namespace

namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

bool MotorDriver::Begin() noexcept
{
    pinMode(Pins::LeftMotorIn1, OUTPUT);
    pinMode(Pins::LeftMotorIn2, OUTPUT);
    pinMode(Pins::LeftMotorPwm, OUTPUT);

    pinMode(Pins::RightMotorIn1, OUTPUT);
    pinMode(Pins::RightMotorIn2, OUTPUT);
    pinMode(Pins::RightMotorPwm, OUTPUT);

    Stop();

    return true;
}

//=============================================================================
// Control
//=============================================================================

void MotorDriver::Stop() noexcept
{
    ApplyMotor(
        Pins::LeftMotorIn1,
        Pins::LeftMotorIn2,
        Pins::LeftMotorPwm,
        MotorConfig::InvertLeftMotor,
        Types::Vehicle::Direction::Stop,
        Types::Vehicle::Turbo::Disabled);

    ApplyMotor(
        Pins::RightMotorIn1,
        Pins::RightMotorIn2,
        Pins::RightMotorPwm,
        MotorConfig::InvertRightMotor,
        Types::Vehicle::Direction::Stop,
        Types::Vehicle::Turbo::Disabled);
}

void MotorDriver::SetLeftMotor(
    Types::Vehicle::Direction direction,
    Types::Vehicle::Turbo turbo) noexcept
{
    ApplyMotor(
        Pins::LeftMotorIn1,
        Pins::LeftMotorIn2,
        Pins::LeftMotorPwm,
        MotorConfig::InvertLeftMotor,
        direction,
        turbo);
}

void MotorDriver::SetRightMotor(
    Types::Vehicle::Direction direction,
    Types::Vehicle::Turbo turbo) noexcept
{
    ApplyMotor(
        Pins::RightMotorIn1,
        Pins::RightMotorIn2,
        Pins::RightMotorPwm,
        MotorConfig::InvertRightMotor,
        direction,
        turbo);
}

//=============================================================================
// Implementación
//=============================================================================

void MotorDriver::ApplyMotor(
    std::uint8_t in1,
    std::uint8_t in2,
    std::uint8_t enable,
    bool invert,
    Types::Vehicle::Direction direction,
    Types::Vehicle::Turbo turbo) noexcept
{
    switch (direction)
    {
        case Types::Vehicle::Direction::Stop:

            digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);
            analogWrite(enable, 0);

            return;

        case Types::Vehicle::Direction::Forward:

            if (!invert)
            {
                digitalWrite(in1, HIGH);
                digitalWrite(in2, LOW);
            }
            else
            {
                digitalWrite(in1, LOW);
                digitalWrite(in2, HIGH);
            }

            break;

        case Types::Vehicle::Direction::Reverse:

            if (!invert)
            {
                digitalWrite(in1, LOW);
                digitalWrite(in2, HIGH);
            }
            else
            {
                digitalWrite(in1, HIGH);
                digitalWrite(in2, LOW);
            }

            break;
    }

    analogWrite(
        enable,
        ToPwm(turbo));
}

} // namespace MK