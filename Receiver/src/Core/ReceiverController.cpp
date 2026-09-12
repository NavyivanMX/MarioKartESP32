/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ReceiverController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador principal del Receiver.
 *
 * Ambiente WTF:
 *
 *   Perfil + Turbo durante 2 segundos
 *       -> entra / sale de WTF
 *
 *   WTF activo:
 *
 *       Perfil + Forward
 *           -> siguiente color
 *
 *       Perfil + Reverse
 *           -> color anterior
 *
 *       Perfil + Left
 *           -> efecto anterior
 *
 *       Perfil + Right
 *           -> siguiente efecto
 *
 * Los comandos continúan siendo procesados normalmente por el vehículo.
 ******************************************************************************/

#include "ReceiverController.h"
#include "src/Config/ReceiverConfig.h"
#include "src/Config/BluetoothConfig.h"

#include <cstring>

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

    if (!m_bluetoothManager.Begin(
        Config::BluetoothConfig::DeviceName))
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
        m_logger.LogError(
            "DrivingProfileManager initialization failed.");

        return false;
    }

    m_logger.LogProfile(
        m_profileManager.Current());

    //-------------------------------------------------------------
    // Ambient Lights
    //-------------------------------------------------------------

    if (!m_ambientLights.Begin())
    {
        m_logger.LogError(
            "Ambient lights initialization failed.");

        return false;
    }

    // Mostrar inicialmente el color del perfil guardado.
    //
    // A partir de aquí comienza el conteo de los 10 segundos.
    //
    m_ambientLights.SetProfile(
        m_profileManager.CurrentId());

    //-------------------------------------------------------------
    // Estado WTF
    //-------------------------------------------------------------

    m_ambientTurboActive = false;
    m_ambientTurboStartedAt = 0;
    m_ambientTurboTriggered = false;

    m_ambientDirectionActive = false;
    m_ambientLastDirection = 0;

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
    //=====================================================================
    // Ambiente
    //=====================================================================
    //
    // NO BLOQUEANTE.
    //
    // Se actualiza aunque en este ciclo no haya llegado un comando.
    //

    m_ambientLights.Update();

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

            m_logger.LogWarning(
                "Communication timeout.");
        }
    }
}

//=============================================================================
// Procesamiento
//=============================================================================

void ReceiverController::ProcessCommand(
    const Protocol::DriverCommand& command) noexcept
{
    //=====================================================================
    // CAMBIO DE PERFIL
    //=====================================================================
    //
    // Gravity continúa siendo el mecanismo de cambio de perfil.
    //
    // Se detecta por FLANCO.
    //

    const bool gravityPressed =
        command.driveMode ==
        Types::Vehicle::DriveMode::Gravity;

    if (gravityPressed &&
        !m_gravityPressedLastFrame)
    {
        //-------------------------------------------------------------
        // Cambiar perfil
        //-------------------------------------------------------------

        m_profileManager.Next();

        m_logger.LogProfile(
            m_profileManager.Current());

        //-------------------------------------------------------------
        // Reiniciar ambiente
        //
        // Nuevo color
        //     ↓
        // 10 segundos
        //     ↓
        // Rainbow
        //-------------------------------------------------------------

        m_ambientLights.SetProfile(
            m_profileManager.CurrentId());

        //-------------------------------------------------------------
        // Una nueva selección de perfil cancela cualquier
        // combinación temporal de WTF.
        //-------------------------------------------------------------

        m_ambientTurboActive = false;
        m_ambientTurboStartedAt = 0;
        m_ambientTurboTriggered = false;

        m_ambientDirectionActive = false;
        m_ambientLastDirection = 0;
    }

    m_gravityPressedLastFrame =
        gravityPressed;

    //=====================================================================
    // AMBIENTE WTF
    //=====================================================================
    //
    // Lo procesamos ANTES del vehículo.
    //
    // Pero NO reemplazamos el comando.
    //
    // El vehículo seguirá recibiendo exactamente el mismo command.
    //

    ProcessAmbientWtf(command);

    //=====================================================================
    // DEBUG
    //=====================================================================

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

        if (m_firstLog ||
            command != m_lastLoggedCommand)
        {
            m_logger.Log(command);

            m_lastLoggedCommand = command;

            m_firstLog = false;
        }
    }

    //=====================================================================
    // VEHÍCULO
    //=====================================================================

    m_vehicle.Update(
        command,
        m_profileManager.Current());

    Serial.print("Current Profile: ");
    Serial.println(
        m_profileManager.Current().name);

    //=====================================================================
    // REAR LIGHTS
    //=====================================================================

    m_rearLights.Update(
        command,
        m_profileManager.Current());

    //=====================================================================
    // Siempre informar el perfil actual al Transmitter
    //=====================================================================

    SendVehicleStatus();

    //=====================================================================
    // Limpiar bandera de cambio
    //=====================================================================

    m_profileManager.ClearProfileChanged();
}

//=============================================================================
// Procesamiento WTF
//=============================================================================

void ReceiverController::ProcessAmbientWtf(
    const Protocol::DriverCommand& command) noexcept
{
    const std::uint32_t now = millis();

    //=====================================================================
    // Detectar Perfil + Turbo
    //=====================================================================

    const bool turboPressed =
        command.turbo ==
        Types::Vehicle::Turbo::Enabled;

    //-------------------------------------------------------------
    // Turbo acaba de presionarse
    //-------------------------------------------------------------

    if (turboPressed)
    {
        //-------------------------------------------------------------
        // Comenzar temporización
        //-------------------------------------------------------------

        if (!m_ambientTurboActive)
        {
            m_ambientTurboActive = true;

            m_ambientTurboStartedAt = now;

            m_ambientTurboTriggered = false;
        }

        //-------------------------------------------------------------
        // ¿Ya lleva 2 segundos?
        //-------------------------------------------------------------

        if (!m_ambientTurboTriggered &&
            (now - m_ambientTurboStartedAt) >=
                AmbientWtfHoldTimeMs)
        {
            //---------------------------------------------------------
            // Toggle WTF
            //---------------------------------------------------------

            m_ambientLights.ToggleWtfMode();

            m_ambientTurboTriggered = true;

            //---------------------------------------------------------
            // Al entrar a WTF comenzamos con estado limpio.
            //---------------------------------------------------------

            m_ambientDirectionActive = false;
            m_ambientLastDirection = 0;
        }

        return;
    }

    //=====================================================================
    // Turbo liberado
    //=====================================================================

    if (m_ambientTurboActive)
    {
        m_ambientTurboActive = false;

        m_ambientTurboStartedAt = 0;

        m_ambientTurboTriggered = false;
    }

    //=====================================================================
    // Si no estamos en WTF no procesamos direcciones.
    //=====================================================================

    if (!m_ambientLights.IsWtfMode())
    {
        m_ambientDirectionActive = false;
        m_ambientLastDirection = 0;

        return;
    }

    //=====================================================================
    // Procesar color / efecto
    //=====================================================================

    ProcessAmbientWtfDirection(command);
}

//=============================================================================
// Procesamiento de dirección WTF
//=============================================================================

void ReceiverController::ProcessAmbientWtfDirection(
    const Protocol::DriverCommand& command) noexcept
{
    //=====================================================================
    // IMPORTANTE
    //=====================================================================
    //
    // No queremos que mantener una dirección genere:
    //
    // ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑
    //
    // cambiando el color continuamente.
    //
    // Solamente actuamos cuando la combinación aparece.
    //

    const auto direction =
        command.direction;

    const auto steering =
        command.steering;

    //=====================================================================
    // FORWARD
    //
    // Perfil + Adelante
    //     -> siguiente color
    //=====================================================================

    if (direction ==
        Types::Vehicle::Direction::Forward)
    {
        if (!m_ambientDirectionActive ||
            m_ambientLastDirection != 1)
        {
            m_ambientLights.NextColor();

            m_ambientDirectionActive = true;

            m_ambientLastDirection = 1;
        }

        return;
    }

    //=====================================================================
    // REVERSE
    //
    // Perfil + Atrás
    //     -> color anterior
    //=====================================================================

    if (direction ==
        Types::Vehicle::Direction::Reverse)
    {
        if (!m_ambientDirectionActive ||
            m_ambientLastDirection != 2)
        {
            m_ambientLights.PreviousColor();

            m_ambientDirectionActive = true;

            m_ambientLastDirection = 2;
        }

        return;
    }

    //=====================================================================
    // LEFT
    //
    // Perfil + Izquierda
    //     -> efecto anterior
    //=====================================================================

    if (steering ==
        Types::Vehicle::Steering::Left)
    {
        if (!m_ambientDirectionActive ||
            m_ambientLastDirection != 3)
        {
            m_ambientLights.PreviousEffect();

            m_ambientDirectionActive = true;

            m_ambientLastDirection = 3;
        }

        return;
    }

    //=====================================================================
    // RIGHT
    //
    // Perfil + Derecha
    //     -> siguiente efecto
    //=====================================================================

    if (steering ==
        Types::Vehicle::Steering::Right)
    {
        if (!m_ambientDirectionActive ||
            m_ambientLastDirection != 4)
        {
            m_ambientLights.NextEffect();

            m_ambientDirectionActive = true;

            m_ambientLastDirection = 4;
        }

        return;
    }

    //=====================================================================
    // Ninguna combinación WTF activa
    //
    // Esto rearma la detección para la siguiente pulsación.
    //=====================================================================

    m_ambientDirectionActive = false;
    m_ambientLastDirection = 0;
}

//=============================================================================
// Enviar estado del vehículo
//=============================================================================

void ReceiverController::SendVehicleStatus() noexcept
{
    Protocol::VehicleStatus status;

    status.drivingProfile =
        m_profileManager.CurrentId();

    m_receiver.SendVehicleStatus(status);

    m_bluetoothManager.Send(status);
}

} // namespace MK