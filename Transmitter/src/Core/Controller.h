/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Controller.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Orquestador principal del transmisor.
 ******************************************************************************/

#ifndef MK_CONTROLLER_H
#define MK_CONTROLLER_H

//=============================================================================
// Includes
//=============================================================================

#include <cstdint>

#include "src/Communication/ESPNowHandler.h"
#include "src/Input/InputManager.h"
#include "src/Debug/ConsoleLogger.h"
#include "src/Status/StatusLightController.h"
#include "src/Config/TransmitterConfig.h"

namespace MK
{

//=============================================================================
// Controller
//=============================================================================

class Controller final
{
public:

    Controller() = default;

    ~Controller() = default;

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    [[nodiscard]]
    bool Begin();

    void Update() noexcept;

private:

    //=========================================================================
    // Comunicación / estado del vehículo
    //=========================================================================

    void UpdateVehicleStatus() noexcept;

    //=========================================================================
    // Estado
    //=========================================================================

    std::uint32_t m_lastTransmitTime = 0;

    Protocol::DriverCommand m_lastLoggedCommand{};

    Protocol::VehicleStatus m_vehicleStatus{};

    bool m_hasVehicleStatus = false;

    //=========================================================================
    // Componentes
    //=========================================================================

    InputManager m_inputManager;

    ESPNowHandler m_espNowHandler;

    StatusLightController m_statusLightController;

    ConsoleLogger m_consoleLogger;
};

} // namespace MK

#endif // MK_CONTROLLER_H