/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotorController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del controlador del sistema de propulsión.
 ******************************************************************************/

#include "src/Vehicle/MotorController.h"

namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

bool MotorController::Begin() noexcept
{
    return m_driver.Begin();
}

//=============================================================================
// Movimiento
//=============================================================================

void MotorController::Drive(
    const Protocol::DriverCommand& command) noexcept
{    
    using Types::Vehicle::Direction;
    using Types::Vehicle::Steering;

    MotorState left
    {
        Direction::Stop,
        command.turbo
    };

    MotorState right
    {
        Direction::Stop,
        command.turbo
    };

    switch (command.direction)
    {
        //---------------------------------------------------------------------
        // Vehículo detenido
        //---------------------------------------------------------------------

        case Direction::Stop:
            break;

        //---------------------------------------------------------------------
        // Avance
        //---------------------------------------------------------------------

        case Direction::Forward:

            switch (command.steering)
            {
                case Steering::Straight:

                    left.direction = Direction::Forward;
                    right.direction = Direction::Forward;
                    break;

                case Steering::Left:

                    left.direction = Direction::Stop;
                    right.direction = Direction::Forward;
                    break;

                case Steering::Right:

                    left.direction = Direction::Forward;
                    right.direction = Direction::Stop;
                    break;
            }

            break;

        //---------------------------------------------------------------------
        // Reversa
        //---------------------------------------------------------------------

        case Direction::Reverse:

            switch (command.steering)
            {
                case Steering::Straight:

                    left.direction = Direction::Reverse;
                    right.direction = Direction::Reverse;
                    break;

                case Steering::Left:

                    left.direction = Direction::Stop;
                    right.direction = Direction::Reverse;
                    break;

                case Steering::Right:

                    left.direction = Direction::Reverse;
                    right.direction = Direction::Stop;
                    break;
            }

            break;
    }

    Apply(
        left,
        right);
}

//=============================================================================
// Aplicación al hardware
//=============================================================================

void MotorController::Apply(
    const MotorState& left,
    const MotorState& right) noexcept
{
    m_driver.SetLeftMotor(
        left.direction,
        left.turbo);

    m_driver.SetRightMotor(
        right.direction,
        right.turbo);
}

} // namespace MK