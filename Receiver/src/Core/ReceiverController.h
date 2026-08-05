/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ReceiverController.h
 ******************************************************************************/

#ifndef MK_RECEIVER_CONTROLLER_H
#define MK_RECEIVER_CONTROLLER_H

//=============================================================================
// Includes
//=============================================================================

#include <cstdint>

#include "src/Communication/BluetoothManager.h"
#include "src/Communication/ESPNowReceiver.h"

#include "src/Debug/ConsoleLogger.h"

#include <Protocol/Protocol.h>

#include "src/Vehicle/VehicleController.h"

#include "src/Vehicle/VehicleProfiles/DrivingProfileManager.h"

namespace MK
{

class ReceiverController final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    [[nodiscard]]
    bool Begin() noexcept;

    //=========================================================================
    // Ejecución
    //=========================================================================

    void Update() noexcept;

private:

    //=========================================================================
    // Procesamiento
    //=========================================================================

    void ProcessCommand(
        const Protocol::DriverCommand& command) noexcept;

private:

    Protocol::DriverCommand m_lastLoggedCommand{};
    bool m_firstLog = true;

    Protocol::DriverCommand m_lastCommand{};

    bool m_hasLastCommand = false;

    void SendVehicleStatus();
    //=========================================================================
    // Debug
    //=========================================================================

    ConsoleLogger m_logger;

    //=========================================================================
    // Comunicación
    //=========================================================================
    std::uint32_t m_lastCommandTime{0};

    bool m_failsafeActive{true};
    
    BluetoothManager m_bluetoothManager;

    ESPNowReceiver m_receiver;

    //=========================================================================
    // Vehículo
    //=========================================================================

    VehicleController m_vehicle;

    //=========================================================================
    // Driving Profiles
    //=========================================================================

    VehicleProfiles::DrivingProfileManager
        m_profileManager;

    //=========================================================================
    // Estado
    //=========================================================================

    bool m_gravityPressedLastFrame{false};

    //=========================================================================
    // Buffers
    //=========================================================================

};

}

#endif