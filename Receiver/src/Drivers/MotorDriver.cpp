/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotorDriver.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del driver de hardware para el sistema de propulsión.
 * Controla ambos motores mediante un puente H TB6612FNG utilizando
 * PWM por hardware (LEDC) del ESP32.
 ******************************************************************************/

#include "src/Drivers/MotorDriver.h"

#include <Arduino.h>

#include "src/Config/MotorConfig.h"
#include "src/Config/Pins.h"

namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

bool MotorDriver::Begin() noexcept
{
    //---------------------------------------------------------------------
    // Pines de dirección
    //---------------------------------------------------------------------

    pinMode(Pins::LeftMotorIn1, OUTPUT);
    pinMode(Pins::LeftMotorIn2, OUTPUT);

    pinMode(Pins::RightMotorIn1, OUTPUT);
    pinMode(Pins::RightMotorIn2, OUTPUT);

    //---------------------------------------------------------------------
    // PWM (Arduino ESP32 Core 3.x)
    //---------------------------------------------------------------------

    ledcAttach(
        Pins::LeftMotorPwm,
        MotorConfig::Frequency,
        MotorConfig::Resolution);

    ledcAttach(
        Pins::RightMotorPwm,
        MotorConfig::Frequency,
        MotorConfig::Resolution);

    Stop();

    return true;
}

//=============================================================================
// Control
//=============================================================================

void MotorDriver::Stop() noexcept
{
    SetLeftMotor(
        Types::Vehicle::Direction::Stop,
        MotorConfig::Power::Stop);

    SetRightMotor(
        Types::Vehicle::Direction::Stop,
        MotorConfig::Power::Stop);
}

void MotorDriver::SetLeftMotor(
    Types::Vehicle::Direction direction,
    std::uint8_t power) noexcept
{
    ApplyMotor(
        Pins::LeftMotorIn1,
        Pins::LeftMotorIn2,
        Pins::LeftMotorPwm,
        MotorConfig::InvertLeftMotor,
        direction,
        power);
}

void MotorDriver::SetRightMotor(
    Types::Vehicle::Direction direction,
    std::uint8_t power) noexcept
{
    ApplyMotor(
        Pins::RightMotorIn1,
        Pins::RightMotorIn2,
        Pins::RightMotorPwm,
        MotorConfig::InvertRightMotor,
        direction,
        power);
}

//=============================================================================
// Implementación
//=============================================================================

void MotorDriver::ApplyMotor(
    std::uint8_t in1,
    std::uint8_t in2,
    std::uint8_t pwmPin,
    bool invert,
    Types::Vehicle::Direction direction,
    std::uint8_t power) noexcept
{
    switch (direction)
    {
        //-----------------------------------------------------------------
        // Stop
        //-----------------------------------------------------------------

        case Types::Vehicle::Direction::Stop:

            digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);

            ledcWrite(
                pwmPin,
                MotorConfig::Power::Stop);

            return;

        //-----------------------------------------------------------------
        // Forward
        //-----------------------------------------------------------------

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

        //-----------------------------------------------------------------
        // Reverse
        //-----------------------------------------------------------------

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

    //---------------------------------------------------------------------
    // Potencia
    //---------------------------------------------------------------------

    ledcWrite(
        pwmPin,
        power);
}

} // namespace MK