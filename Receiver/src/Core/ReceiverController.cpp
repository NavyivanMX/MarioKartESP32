/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ReceiverController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador principal del Receiver.
 ******************************************************************************/

#include "ReceiverController.h"
#include "src/Config/ReceiverConfig.h"
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
    // Rear Lights
    //-------------------------------------------------------------

    if (!m_rearLights.Begin())
    {
        m_logger.LogError("Rear lights initialization failed.");
        return false;
    }



    //-------------------------------------------------------------
    // Vehículo
    //-------------------------------------------------------------

    if (!m_vehicle.Begin())
    {
        m_logger.LogError("Vehicle initialization failed.");
        return false;
    }

    //-------------------------------------------------------------
    // Bluetooth
    //-------------------------------------------------------------

    if (!m_bluetoothManager.Begin(Config::BluetoothConfig::DeviceName))
    {
        m_logger.LogError("Bluetooth initialization failed.");
        return false;
    }

    //-------------------------------------------------------------
    // ESP-NOW
    //-------------------------------------------------------------

    if (!m_receiver.Begin())
    {
        m_logger.LogError("ESP-NOW initialization failed.");
        return false;
    }

    //-------------------------------------------------------------
    // Driving Profiles
    //-------------------------------------------------------------

    if (!m_profileManager.Begin())
    {
        m_logger.LogError("DrivingProfileManager initialization failed.");          
        return false;
    }

    m_logger.LogProfile(m_profileManager.Current());

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
        m_lastCommandTime = millis();
        m_failsafeActive = false;
        ProcessCommand(command);
        return;
    }

    //-------------------------------------------------------------
    // ESP-NOW
    //-------------------------------------------------------------

    if (m_receiver.Receive(command))
    {
        m_lastCommandTime = millis();
        m_failsafeActive = false;
        ProcessCommand(command);
        return;
    }

    //-------------------------------------------------------------
    // Failsafe
    //-------------------------------------------------------------

    if (!m_failsafeActive)
    {
        if ((millis() - m_lastCommandTime) >
            ReceiverConfig::CommandTimeoutMs)
        {
            m_vehicle.Stop();
            m_rearLights.Stop();
            m_failsafeActive = true;

            m_logger.LogWarning("Communication timeout.");
        }
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

            m_logger.LogProfile(m_profileManager.Current());
        }

        m_gravityPressedLastFrame =
            gravityPressed;

        //-------------------------------------------------------------
        // Debug
        //-------------------------------------------------------------

        bool commandChanged =
            !m_hasLastCommand ||
            std::memcmp(
                &command,
                &m_lastCommand,
                sizeof(command)) != 0;

        if (commandChanged)
        {
            m_lastCommand = command;
            m_hasLastCommand = true;

            if (m_firstLog || command != m_lastLoggedCommand)
            {
                m_logger.Log(command);
                m_lastLoggedCommand = command;
                m_firstLog = false;
            }
        }

        //-------------------------------------------------------------
        // Vehículo
        //-------------------------------------------------------------

        m_vehicle.Update(command,m_profileManager.Current());

        Serial.print("Current Profile: ");
        Serial.println( m_profileManager.Current().name);

        m_rearLights.Update(command,m_profileManager.Current());
        //-------------------------------------------------------------
        // Siempre informar el perfil actual al Transmitter
        //-------------------------------------------------------------

        SendVehicleStatus();

        //-------------------------------------------------------------
        // Limpiar bandera de cambio
        //-------------------------------------------------------------

        m_profileManager.ClearProfileChanged();
    }
    void ReceiverController::SendVehicleStatus() noexcept
    {
        Protocol::VehicleStatus status;

        status.drivingProfile =
            m_profileManager.CurrentId();

        m_bluetoothManager.Send(status);
    }
} // namespace MK