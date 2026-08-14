/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ReceiverController.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador principal del Receiver.
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
#include "src/Vehicle/RearLightController.h"

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

    //=========================================================================
    // Estado del vehículo
    //=========================================================================

    void SendVehicleStatus() noexcept;

private:

    //=========================================================================
    // Debug
    //=========================================================================

    ConsoleLogger m_logger;

    Protocol::DriverCommand m_lastLoggedCommand{};
    bool m_firstLog = true;

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

    RearLightController m_rearLights;

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
    // Último comando
    //=========================================================================

    Protocol::DriverCommand m_lastCommand{};

    bool m_hasLastCommand{false};
};

} // namespace MK

#endif // MK_RECEIVER_CONTROLLER_H