/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowHandler.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Encapsula la inicialización y comunicación mediante ESP-NOW.
 *
 * Permite:
 *   - Trabajar con dos receptores posibles.
 *   - Detectar cuál está disponible al iniciar.
 *   - Seleccionar un único receptor.
 *   - Mantener el receptor seleccionado durante toda la sesión.
 ******************************************************************************/

#ifndef MK_ESP_NOW_HANDLER_H
#define MK_ESP_NOW_HANDLER_H

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

    [[nodiscard]]
    bool Begin();

    void End() noexcept;

    [[nodiscard]]
    bool Send(
        const std::uint8_t* packet,
        std::size_t length) noexcept;

    [[nodiscard]]
    bool ReceiveVehicleStatus(
        Protocol::VehicleStatus& status) noexcept;

private:

    //=========================================================================
    // Callbacks ESP-NOW
    //=========================================================================

    static void OnDataSent(
        const wifi_tx_info_t* txInfo,
        esp_now_send_status_t status) noexcept;

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
    bool RegisterPeers() noexcept;

    [[nodiscard]]
    bool RegisterPeer(
        const Types::MacAddress& macAddress) noexcept;

    [[nodiscard]]
    bool IsInitialized() const noexcept;


    //=========================================================================
    // Receptores
    //=========================================================================

    [[nodiscard]]
    bool IsValidMac(
        const Types::MacAddress& macAddress) const noexcept;

    [[nodiscard]]
    bool IsConfiguredReceiver(
        const Types::MacAddress& macAddress) const noexcept;

    void SelectReceiver(
        const Types::MacAddress& macAddress) noexcept;


    //=========================================================================
    // Estado de transmisión / selección
    //=========================================================================

    static constexpr std::uint8_t KartReceiverIndex = 0;
    static constexpr std::uint8_t LabReceiverIndex = 1;

    std::uint8_t m_receiverCandidate = KartReceiverIndex;

    Types::MacAddress m_selectedReceiver{};

    bool m_receiverSelected = false;

    volatile bool m_transmissionPending = false;
    volatile bool m_transmissionResultAvailable = false;
    volatile bool m_lastTransmissionSuccessful = false;


    //=========================================================================
    // Estado ESP-NOW
    //=========================================================================

    esp_now_peer_info_t m_peerKart{};
    esp_now_peer_info_t m_peerLab{};

    bool m_initialized = false;


    //=========================================================================
    // VehicleStatus
    //=========================================================================

    Protocol::VehicleStatus m_lastVehicleStatus{};
    bool m_vehicleStatusAvailable = false;


    //=========================================================================
    // Instancia estática para callbacks
    //=========================================================================

    static ESPNowHandler* s_instance;
};

} // namespace MK

#endif // MK_ESP_NOW_HANDLER_H