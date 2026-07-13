/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowHandler.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Encapsula la inicialización y comunicación mediante ESP-NOW.
 ******************************************************************************/

#ifndef MK_ESP_NOW_HANDLER_H
#define MK_ESP_NOW_HANDLER_H

#include <WiFi.h>

#include <esp_err.h>
#include <esp_now.h>

#include "src/Shared/Protocol.h"

namespace MK
{

class ESPNowHandler final
{
public:

    ESPNowHandler() = default;

    ~ESPNowHandler() = default;

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    [[nodiscard]]
    bool Begin();

    void End() noexcept;

    //=========================================================================
    // Comunicación
    //=========================================================================

    [[nodiscard]]
    bool Send(const Protocol::DriverCommand& command) noexcept;

private:

    [[nodiscard]]
    bool InitializeWiFi() noexcept;

    [[nodiscard]]
    bool InitializeESPNow() noexcept;

    [[nodiscard]]
    bool RegisterPeer() noexcept;

    [[nodiscard]]
    bool IsInitialized() const noexcept;

private:

    esp_now_peer_info_t m_peer{};

    bool m_initialized = false;
};

} // namespace MK

#endif // MK_ESP_NOW_HANDLER_H