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
    const Protocol::DriverCommand& command,
    const VehicleProfiles::DrivingProfile& profile) noexcept
{
    using Types::Vehicle::Direction;
    using Types::Vehicle::Steering;
    using Types::Vehicle::Turbo;
    //---------------------------------------------------------------------
    // Potencias según el perfil seleccionado
    //---------------------------------------------------------------------

    const std::uint8_t forwardPower =
        (command.turbo == Turbo::Enabled &&
         profile.turboEnabled)
            ? profile.turboSpeed
            : profile.forwardSpeed;

    const std::uint8_t forwardTurnPower =
        static_cast<std::uint8_t>(
            forwardPower *
            profile.steeringFactor);

    const std::uint8_t reversePower =
        profile.reverseSpeed;

    const std::uint8_t reverseTurnPower =
        static_cast<std::uint8_t>(
            reversePower *
            profile.steeringFactor);

    //---------------------------------------------------------------------
    // Estado inicial
    //---------------------------------------------------------------------

    MotorState left{};
    MotorState right{};

    left.direction = Direction::Stop;
    right.direction = Direction::Stop;

    left.power = 0;
    right.power = 0;

    Serial.printf(
    "Forward=%d\n",
    forwardPower);

    //---------------------------------------------------------------------
    // Movimiento
    //---------------------------------------------------------------------

    switch (command.direction)
    {
        //-------------------------------------------------------------
        // Stop
        //-------------------------------------------------------------

        case Direction::Stop:

            switch (command.steering)
            {
                case Steering::Straight:
                    break;

                case Steering::Left:

                    if (MotorConfig::Steering::PivotTurnEnabled)
                    {
                        left.direction = Direction::Reverse;
                        right.direction = Direction::Forward;

                        left.power = forwardPower;
                        right.power = forwardPower;
                    }

                    break;

                case Steering::Right:

                    if (MotorConfig::Steering::PivotTurnEnabled)
                    {
                        left.direction = Direction::Forward;
                        right.direction = Direction::Reverse;

                        left.power = forwardPower;
                        right.power = forwardPower;
                    }

                    break;
            }

            break;

        //-------------------------------------------------------------
        // Forward
        //-------------------------------------------------------------

        case Direction::Forward:

            switch (command.steering)
            {
                case Steering::Straight:

                    left.direction = Direction::Forward;
                    right.direction = Direction::Forward;

                    left.power = forwardPower;
                    right.power = forwardPower;

                    break;

                case Steering::Left:

                    left.direction = Direction::Forward;
                    right.direction = Direction::Forward;

                    left.power = forwardTurnPower;
                    right.power = forwardPower;

                    break;

                case Steering::Right:

                    left.direction = Direction::Forward;
                    right.direction = Direction::Forward;

                    left.power = forwardPower;
                    right.power = forwardTurnPower;

                    break;
            }

            break;

        //-------------------------------------------------------------
        // Reverse
        //-------------------------------------------------------------

        case Direction::Reverse:

            switch (command.steering)
            {
                case Steering::Straight:

                    left.direction = Direction::Reverse;
                    right.direction = Direction::Reverse;

                    left.power = reversePower;
                    right.power = reversePower;

                    break;

                case Steering::Left:

                    left.direction = Direction::Reverse;
                    right.direction = Direction::Reverse;

                    left.power = reverseTurnPower;
                    right.power = reversePower;

                    break;

                case Steering::Right:

                    left.direction = Direction::Reverse;
                    right.direction = Direction::Reverse;

                    left.power = reversePower;
                    right.power = reverseTurnPower;

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