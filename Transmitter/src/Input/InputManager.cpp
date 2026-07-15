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

void InputManager::Begin() noexcept
{
    ForEachButton(
        [](Button& button)
        {
            button.Begin();
        });

    BuildDriverCommand();
}

bool InputManager::Update() noexcept
{
    ReadButtons();

    return BuildDriverCommand();
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

void InputManager::ReadButtons() noexcept
{
    ForEachButton(
        [](Button& button)
        {
            button.Update();
        });
}

bool InputManager::BuildDriverCommand() noexcept
{
    Protocol::DriverCommand command{};

    //---------------------------------------------------------------------
    // Dirección longitudinal
    //---------------------------------------------------------------------

    if (m_forwardButton.IsPressed())
    {
        command.direction =
            Types::Vehicle::Direction::Forward;
    }
    else if (m_reverseButton.IsPressed())
    {
        command.direction =
            Types::Vehicle::Direction::Reverse;
    }
    else
    {
        command.direction =
            Types::Vehicle::Direction::Stop;
    }

    //---------------------------------------------------------------------
    // Dirección lateral
    //---------------------------------------------------------------------

    if (m_leftButton.IsPressed())
    {
        command.steering =
            Types::Vehicle::Steering::Left;
    }
    else if (m_rightButton.IsPressed())
    {
        command.steering =
            Types::Vehicle::Steering::Right;
    }
    else
    {
        command.steering =
            Types::Vehicle::Steering::Straight;
    }

    //---------------------------------------------------------------------
    // Turbo
    //---------------------------------------------------------------------

    command.turbo =
        m_turboButton.IsPressed()
            ? Types::Vehicle::Turbo::Enabled
            : Types::Vehicle::Turbo::Disabled;

    //---------------------------------------------------------------------
    // Drive Mode
    //---------------------------------------------------------------------

    command.driveMode =
        m_gravityButton.IsPressed()
            ? Types::Vehicle::DriveMode::Gravity
            : Types::Vehicle::DriveMode::Normal;

    //---------------------------------------------------------------------
    // Detectar cambios
    //---------------------------------------------------------------------

    if (command == m_driverCommand)
    {
        return false;
    }

    m_driverCommand = command;

    return true;
}

} // namespace MK