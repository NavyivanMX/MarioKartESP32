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

#include <cstdint>

#include "src/Communication/ESPNowHandler.h"
#include "src/Input/InputManager.h"
#include "src/Debug/ConsoleLogger.h"
#include "src/Status/StatusLightController.h"
#include "src/Config/TransmitterConfig.h"
#include "src/Config/TransmitterPersistentConfig.h"

namespace MK
{

class Controller final
{
public:

    Controller() = default;

    ~Controller() = default;


    [[nodiscard]]
    bool Begin();


    void Update() noexcept;


private:

    void UpdateVehicleStatus() noexcept;


    //=========================================================================
    // Configuración por Serial
    //=========================================================================

    void ProcessSerialConfiguration() noexcept;


    bool ParseMacAddress(
        const char* text,
        Types::MacAddress& macAddress) noexcept;


    void PrintMacAddress(
        const Types::MacAddress& macAddress) noexcept;


private:

    std::uint32_t m_lastTransmitTime = 0;


    Protocol::DriverCommand m_lastLoggedCommand{};


    Protocol::VehicleStatus m_vehicleStatus{};


    bool m_hasVehicleStatus = false;


    InputManager m_inputManager;


    ESPNowHandler m_espNowHandler;


    StatusLightController m_statusLightController;


    ConsoleLogger m_consoleLogger;


    // Configuración persistente del Transmitter
    TransmitterPersistentConfig m_persistentConfig;


    // Buffer para comandos MKCFG recibidos por Serial
    char m_serialBuffer[80]{};

    std::size_t m_serialBufferLength = 0;
};

}

#endif