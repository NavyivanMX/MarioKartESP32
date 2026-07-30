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

    if (!m_bluetooth.Initialize(
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
    // Ahora ya existe la interfaz WiFi
    //-------------------------------------------------------------

    m_logger.LogBoot();

    m_logger.LogBluetooth(
        Config::BluetoothConfig::DeviceName);

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
    //-------------------------------------------------------------
    // Bluetooth tiene prioridad
    //-------------------------------------------------------------

    Protocol::DriverCommand command;

    if (m_bluetooth.Receive(command))
    {
        m_logger.Log(command);

        m_vehicle.Update(command);

        return;
    }

    //-------------------------------------------------------------
    // ESP-NOW
    //-------------------------------------------------------------

    if (m_receiver.Receive(command))
    {
        m_logger.Log(command);

        m_vehicle.Update(command);
    }
}

} // namespace MK