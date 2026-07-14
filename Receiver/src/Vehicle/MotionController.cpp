/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotionController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del controlador de movimiento del vehículo.
 ******************************************************************************/

#include "src/Vehicle/MotionController.h"

namespace MK
{

void MotionController::Initialize() noexcept
{
    m_motor.Initialize();
}

void MotionController::Update(
    const Protocol::DriverCommand& command) noexcept
{
    using namespace Types::Vehicle;

    switch (command.direction)
    {
        case Direction::Stop:

            switch (command.steering)
            {
                case Steering::Straight:
                    Stop();
                    break;

                case Steering::Left:
                    RotateLeft(command.turbo);
                    break;

                case Steering::Right:
                    RotateRight(command.turbo);
                    break;
            }

            break;

        case Direction::Forward:

            switch (command.steering)
            {
                case Steering::Straight:
                    Forward(command.turbo);
                    break;

                case Steering::Left:
                    ForwardLeft(command.turbo);
                    break;

                case Steering::Right:
                    ForwardRight(command.turbo);
                    break;
            }

            break;

        case Direction::Reverse:

            switch (command.steering)
            {
                case Steering::Straight:
                    Backward(command.turbo);
                    break;

                case Steering::Left:
                    BackwardLeft(command.turbo);
                    break;

                case Steering::Right:
                    BackwardRight(command.turbo);
                    break;
            }

            break;
    }
}

void MotionController::Stop() noexcept
{
    m_motor.Update(
        {
            MotorDirection::Stop,
            Types::Vehicle::Turbo::Off
        },
        {
            MotorDirection::Stop,
            Types::Vehicle::Turbo::Off
        });
}

void MotionController::Forward(
    Types::Vehicle::Turbo turbo) noexcept
{
    m_motor.Update(
        {
            MotorDirection::Forward,
            turbo
        },
        {
            MotorDirection::Forward,
            turbo
        });
}

void MotionController::Backward(
    Types::Vehicle::Turbo turbo) noexcept
{
    m_motor.Update(
        {
            MotorDirection::Reverse,
            turbo
        },
        {
            MotorDirection::Reverse,
            turbo
        });
}

void MotionController::RotateLeft(
    Types::Vehicle::Turbo turbo) noexcept
{
    m_motor.Update(
        {
            MotorDirection::Reverse,
            turbo
        },
        {
            MotorDirection::Forward,
            turbo
        });
}

void MotionController::RotateRight(
    Types::Vehicle::Turbo turbo) noexcept
{
    m_motor.Update(
        {
            MotorDirection::Forward,
            turbo
        },
        {
            MotorDirection::Reverse,
            turbo
        });
}

void MotionController::ForwardLeft(
    Types::Vehicle::Turbo turbo) noexcept
{
    m_motor.Update(
        {
            MotorDirection::Forward,
            Types::Vehicle::Turbo::Off
        },
        {
            MotorDirection::Forward,
            turbo
        });
}

void MotionController::ForwardRight(
    Types::Vehicle::Turbo turbo) noexcept
{
    m_motor.Update(
        {
            MotorDirection::Forward,
            turbo
        },
        {
            MotorDirection::Forward,
            Types::Vehicle::Turbo::Off
        });
}

void MotionController::BackwardLeft(
    Types::Vehicle::Turbo turbo) noexcept
{
    m_motor.Update(
        {
            MotorDirection::Reverse,
            Types::Vehicle::Turbo::Off
        },
        {
            MotorDirection::Reverse,
            turbo
        });
}

void MotionController::BackwardRight(
    Types::Vehicle::Turbo turbo) noexcept
{
    m_motor.Update(
        {
            MotorDirection::Reverse,
            turbo
        },
        {
            MotorDirection::Reverse,
            Types::Vehicle::Turbo::Off
        });
}

} // namespace MK