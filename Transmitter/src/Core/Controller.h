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

#include "src/Communication/ESPNowHandler.h"
#include "src/Input/InputManager.h"
#include "src/Debug/ConsoleLogger.h"
#include "src/Config/TransmitterConfig.h"

//#include <MKShared.h>

namespace MK
{

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

    std::uint32_t m_lastTransmitTime = 0;

    Protocol::DriverCommand m_lastLoggedCommand{};

    InputManager m_inputManager;

    ESPNowHandler m_espNowHandler;

    ConsoleLogger m_consoleLogger;
};

} // namespace MK

#endif // MK_CONTROLLER_H