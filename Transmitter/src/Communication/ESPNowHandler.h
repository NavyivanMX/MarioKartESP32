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

//=============================================================================
// ESPNowHandler
//=============================================================================

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

    /**
     * @brief Envía un paquete binario mediante ESP-NOW.
     */
    [[nodiscard]]
    bool Send(
        const std::uint8_t* packet,
        std::size_t length) noexcept;

    //=========================================================================
    // Comunicación - RX
    //=========================================================================

    /**
     * @brief Intenta obtener el último VehicleStatus recibido.
     *
     * @param status Estructura donde se almacenará el estado.
     *
     * @return true si existe un estado nuevo disponible.
     */
    [[nodiscard]]
    bool ReceiveVehicleStatus(
        Protocol::VehicleStatus& status) noexcept;

private:

    //=========================================================================
    // Callback ESP-NOW
    //=========================================================================

    static void OnDataReceive(
        const esp_now_recv_info_t* info,
        const std::uint8_t* data,
        int length) noexcept;

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
    // Estado recibido
    //=========================================================================

    Protocol::VehicleStatus m_lastVehicleStatus{};

    bool m_vehicleStatusAvailable = false;

    //=========================================================================
    // Instancia activa
    //=========================================================================

    static ESPNowHandler* s_instance;
};

} // namespace MK

#endif // MK_ESP_NOW_HANDLER_H