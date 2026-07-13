/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : InputManager.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del administrador de entradas del transmisor.
 ******************************************************************************/

#include "InputManager.h"

namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

void InputManager::Begin()
{
    ForEachButton(
        [](Button& button)
        {
            button.Begin();
        });

    BuildDriverCommand();
}

void InputManager::Update()
{
    ReadButtons();

    BuildDriverCommand();
}

//=============================================================================
// Acceso
//=============================================================================

const Protocol::DriverCommand&
InputManager::GetDriverCommand() const noexcept
{
    return m_driverCommand;
}

//=============================================================================
// Actualización
//=============================================================================

void InputManager::ReadButtons()
{
    ForEachButton(
        [](Button& button)
        {
            button.Update();
        });
}

void InputManager::BuildDriverCommand()
{
    //
    // Dirección longitudinal
    //
    if (m_forwardButton.IsPressed())
    {
        m_driverCommand.direction =
            Types::Vehicle::Direction::Forward;
    }
    else if (m_reverseButton.IsPressed())
    {
        m_driverCommand.direction =
            Types::Vehicle::Direction::Reverse;
    }
    else
    {
        m_driverCommand.direction =
            Types::Vehicle::Direction::Stop;
    }

    //
    // Dirección lateral
    //
    if (m_leftButton.IsPressed())
    {
        m_driverCommand.steering =
            Types::Vehicle::Steering::Left;
    }
    else if (m_rightButton.IsPressed())
    {
        m_driverCommand.steering =
            Types::Vehicle::Steering::Right;
    }
    else
    {
        m_driverCommand.steering =
            Types::Vehicle::Steering::Straight;
    }

    //
    // Turbo
    //
    m_driverCommand.turbo =
        m_turboButton.IsPressed()
            ? Types::Vehicle::Turbo::Enabled
            : Types::Vehicle::Turbo::Disabled;

    m_driverCommand.driveMode =
        m_gravityButton.IsPressed()
            ? Types::Vehicle::DriveMode::Gravity
            : Types::Vehicle::DriveMode::Normal;
    }

} // namespace MK