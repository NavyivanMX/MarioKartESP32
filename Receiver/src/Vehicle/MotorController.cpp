/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotorController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del controlador del sistema de propulsión.
 ******************************************************************************/

#include "src/Vehicle/MotorController.h"

#include "src/Config/MotorConfig.h"

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
    using Types::Vehicle::Turbo;

    //---------------------------------------------------------------------
    // Potencia base
    //---------------------------------------------------------------------

    const std::uint8_t cruisePower =
        (command.turbo == Turbo::Enabled)
            ? MotorConfig::Power::Turbo
            : MotorConfig::Power::Cruise;

    const std::uint8_t turnPower =
        static_cast<std::uint8_t>(
            cruisePower *
            MotorConfig::Steering::TurnRatio);

    //---------------------------------------------------------------------
    // Estado inicial
    //---------------------------------------------------------------------

    MotorState left;
    MotorState right;

    left.direction = Direction::Stop;
    right.direction = Direction::Stop;

    left.power = MotorConfig::Power::Stop;
    right.power = MotorConfig::Power::Stop;

    //---------------------------------------------------------------------
    // Movimiento
    //---------------------------------------------------------------------

    switch (command.direction)
    {
        //-----------------------------------------------------------------
        // Vehículo detenido
        //-----------------------------------------------------------------

        case Direction::Stop:

            switch (command.steering)
            {
                case Steering::Straight:
                    break;

                //---------------------------------------------------------
                // Giro tipo tanque
                //---------------------------------------------------------

                case Steering::Left:

                    if (MotorConfig::Steering::PivotTurnEnabled)
                    {
                        left.direction = Direction::Reverse;
                        right.direction = Direction::Forward;

                        left.power = cruisePower;
                        right.power = cruisePower;
                    }

                    break;

                case Steering::Right:

                    if (MotorConfig::Steering::PivotTurnEnabled)
                    {
                        left.direction = Direction::Forward;
                        right.direction = Direction::Reverse;

                        left.power = cruisePower;
                        right.power = cruisePower;
                    }

                    break;
            }

            break;

        //-----------------------------------------------------------------
        // Avance
        //-----------------------------------------------------------------

        case Direction::Forward:

            switch (command.steering)
            {
                case Steering::Straight:

                    left.direction = Direction::Forward;
                    right.direction = Direction::Forward;

                    left.power = cruisePower;
                    right.power = cruisePower;

                    break;

                case Steering::Left:

                    left.direction = Direction::Forward;
                    right.direction = Direction::Forward;

                    left.power = turnPower;
                    right.power = cruisePower;

                    break;

                case Steering::Right:

                    left.direction = Direction::Forward;
                    right.direction = Direction::Forward;

                    left.power = cruisePower;
                    right.power = turnPower;

                    break;
            }

            break;

        //-----------------------------------------------------------------
        // Reversa
        //-----------------------------------------------------------------

        case Direction::Reverse:

            switch (command.steering)
            {
                case Steering::Straight:

                    left.direction = Direction::Reverse;
                    right.direction = Direction::Reverse;

                    left.power = cruisePower;
                    right.power = cruisePower;

                    break;

                case Steering::Left:

                    left.direction = Direction::Reverse;
                    right.direction = Direction::Reverse;

                    left.power = turnPower;
                    right.power = cruisePower;

                    break;

                case Steering::Right:

                    left.direction = Direction::Reverse;
                    right.direction = Direction::Reverse;

                    left.power = cruisePower;
                    right.power = turnPower;

                    break;
            }

            break;
    }

    //---------------------------------------------------------------------
    // Aplicar al hardware
    //---------------------------------------------------------------------

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
        left.power);

    m_driver.SetRightMotor(
        right.direction,
        right.power);
}

} // namespace MK