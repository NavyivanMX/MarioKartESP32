/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ReceiverController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador principal del Receiver.
 ******************************************************************************/

#include "ReceiverController.h"

#include "src/Config/BluetoothConfig.h"

namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

bool ReceiverController::Begin() noexcept
{
    //-------------------------------------------------------------
    // Consola
    //-------------------------------------------------------------

    m_logger.Begin();

    m_logger.LogBoot();

    //-------------------------------------------------------------
    // Vehículo
    //-------------------------------------------------------------

    if (!m_vehicle.Begin())
    {
        m_logger.LogError(
            "Vehicle initialization failed.");

        return false;
    }

    //-------------------------------------------------------------
    // Bluetooth
    //-------------------------------------------------------------

    if (!m_bluetoothManager.Begin(
            Config::BluetoothConfig::DeviceName))
    {
        m_logger.LogError(
            "Bluetooth initialization failed.");

        return false;
    }

    //-------------------------------------------------------------
    // ESP-NOW
    //-------------------------------------------------------------

    if (!m_receiver.Begin())
    {
        m_logger.LogError(
            "ESP-NOW initialization failed.");

        return false;
    }

    //-------------------------------------------------------------
    // Driving Profiles
    //-------------------------------------------------------------

    if (!m_profileManager.Begin())
    {
        m_logger.LogError(
            "DrivingProfileManager initialization failed.");

        return false;
    }

    m_logger.LogProfile(
        m_profileManager.Current());

    //-------------------------------------------------------------
    // Sistema listo
    //-------------------------------------------------------------

    m_logger.LogReady();

    return true;
}

//=============================================================================
// Update
//=============================================================================

void ReceiverController::Update() noexcept
{
    Protocol::DriverCommand command;

    //-------------------------------------------------------------
    // Bluetooth
    //-------------------------------------------------------------

    if (m_bluetoothManager.Receive(command))
    {
        ProcessCommand(command);
        return;
    }

    //-------------------------------------------------------------
    // ESP-NOW
    //-------------------------------------------------------------

    if (m_receiver.Receive(command))
    {
        ProcessCommand(command);
    }
}

//=============================================================================
// Procesamiento
//=============================================================================

void ReceiverController::ProcessCommand(
    const Protocol::DriverCommand& command) noexcept
{
    //-------------------------------------------------------------
    // Cambio de perfil (flanco)
    //-------------------------------------------------------------

    const bool gravityPressed =
        command.driveMode ==
        Types::Vehicle::DriveMode::Gravity;

    if (gravityPressed &&
        !m_gravityPressedLastFrame)
    {
        m_profileManager.Next();

        m_logger.LogProfile(
            m_profileManager.Current());
    }

    m_gravityPressedLastFrame =
        gravityPressed;

    //-------------------------------------------------------------
    // Debug
    //-------------------------------------------------------------

    m_logger.Log(command);

    //-------------------------------------------------------------
    // Vehículo
    //-------------------------------------------------------------

    m_vehicle.Update(
        command,
        m_profileManager.Current());

    if (m_profileManager.ProfileChanged())
    {
        SendVehicleStatus();

        m_profileManager.ClearProfileChanged();
    }        
}
    void ReceiverController::SendVehicleStatus()
    {
        Protocol::VehicleStatus status;

        status.drivingProfile =
            m_profileManager.CurrentId();

        m_bluetoothManager.Send(status);
    }
} // namespace MK