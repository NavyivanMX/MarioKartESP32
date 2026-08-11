/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowHandler.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Encapsula la inicialización y comunicación mediante ESP-NOW.
 * Permite enviar DriverCommand y recibir VehicleStatus.
 ******************************************************************************/

#ifndef MK_ESP_NOW_HANDLER_H
#define MK_ESP_NOW_HANDLER_H

//=============================================================================
// Includes
//=============================================================================

#include <cstddef>
#include <cstdint>

#include <WiFi.h>
#include <esp_now.h>

#include <MKShared.h>

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
    // Comunicación - TX
    //=========================================================================

    /// Envía un paquete binario mediante ESP-NOW.
    [[nodiscard]]
    bool Send(
        const std::uint8_t* packet,
        std::size_t length) noexcept;

    //=========================================================================
    // Comunicación - RX
    //=========================================================================

    /**
     * @brief Intenta recibir un VehicleStatus pendiente.
     *
     * @param status Estructura donde se almacenará el estado recibido.
     *
     * @return true si se recibió un VehicleStatus válido.
     */
    [[nodiscard]]
    bool ReceiveVehicleStatus(
        Protocol::VehicleStatus& status) noexcept;

private:

    //=========================================================================
    // Inicialización
    //=========================================================================

    [[nodiscard]]
    bool InitializeWiFi() noexcept;

    [[nodiscard]]
    bool InitializeESPNow() noexcept;

    [[nodiscard]]
    bool RegisterPeer() noexcept;

    [[nodiscard]]
    bool IsInitialized() const noexcept;

private:

    //=========================================================================
    // Estado ESP-NOW
    //=========================================================================

    esp_now_peer_info_t m_peer{};

    bool m_initialized = false;

    //=========================================================================
    // RX
    //=========================================================================

    Protocol::VehicleStatus m_lastVehicleStatus{};

    bool m_vehicleStatusAvailable = false;
};

} // namespace MK

#endif // MK_ESP_NOW_HANDLER_H