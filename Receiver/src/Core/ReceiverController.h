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

#include "src/Communication/BluetoothTransport.h"
#include "src/Communication/ESPNowReceiver.h"

#include "src/Debug/ConsoleLogger.h"

#include <Protocol/Protocol.h>
#include <Protocol/DriverCommandSerializer.h>

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

    //=========================================================================
    // Debug
    //=========================================================================

    ConsoleLogger m_logger;

    //=========================================================================
    // Comunicación
    //=========================================================================

    BluetoothTransport m_bluetooth;

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

    std::uint8_t m_packet[
        Protocol::DriverCommandSerializer::PacketSize];
};

}

#endif