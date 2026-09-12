/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ReceiverController.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador principal del Receiver.
 *
 * Ambiente WTF:
 *   Perfil + Turbo mantenido durante 2 segundos:
 *       → Entra / sale del modo WTF.
 *
 *   WTF activo:
 *       Perfil + Adelante/Atrás
 *           → Cambia color.
 *
 *       Perfil + Izquierda/Derecha
 *           → Cambia efecto.
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
#include "src/Vehicle/AmbientLightController.h"

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
    // Ambiente WTF
    //=========================================================================

    // Procesa la combinación Perfil + Turbo.
    void ProcessAmbientWtf(
        const Protocol::DriverCommand& command) noexcept;

    // Procesa las direcciones cuando WTF está activo.
    void ProcessAmbientWtfDirection(
        const Protocol::DriverCommand& command) noexcept;

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

    AmbientLightController m_ambientLights;

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

     // Estado del vehículo
        void SendVehicleStatus() noexcept;
    //=========================================================================
    // Ambiente WTF
    //=========================================================================

    // Indica si Turbo está siendo mantenido junto con Perfil.
    bool m_ambientTurboActive{false};

    // Momento en que comenzó a mantenerse Perfil + Turbo.
    std::uint32_t m_ambientTurboStartedAt{0};

    // Evita que la combinación vuelva a dispararse mientras
    // el usuario continúa manteniendo presionado Turbo.
    bool m_ambientTurboTriggered{false};

    // Tiempo necesario para activar/desactivar WTF.
    static constexpr std::uint32_t AmbientWtfHoldTimeMs = 2000;

    //=========================================================================
    // Estado de las direcciones WTF
    //=========================================================================

    // Evita repetir el cambio de color continuamente mientras
    // se mantiene la dirección.
    bool m_ambientDirectionActive{false};

    // Última dirección procesada en WTF.
    std::int8_t m_ambientLastDirection{0};
};

} // namespace MK

#endif // MK_RECEIVER_CONTROLLER_H
