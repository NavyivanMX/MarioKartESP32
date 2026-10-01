/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowHandler.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Encapsula la inicialización y comunicación mediante ESP-NOW.
 *
 * Selección de Receiver:
 *
 *   1. Recorre KnownReceiverMacs[] en orden.
 *   2. Si alguno responde, queda seleccionado.
 *   3. Si todos fallan, intenta el MAC personalizado de NVS.
 *   4. Una vez seleccionado un Receiver, se mantiene durante la sesión.
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


    //=========================================================================
    // Inicialización
    //=========================================================================

    [[nodiscard]]
    bool Begin(
        bool hasCustomReceiverMac,
        const Types::MacAddress& customReceiverMac);


    void End() noexcept;


    //=========================================================================
    // Comunicación
    //=========================================================================

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
    // Inicialización interna
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


    //=========================================================================
    // Selección de Receiver
    //=========================================================================

    [[nodiscard]]
    bool IsValidMac(
        const Types::MacAddress& macAddress) const noexcept;


    [[nodiscard]]
    bool IsConfiguredReceiver(
        const Types::MacAddress& macAddress) const noexcept;


    [[nodiscard]]
    bool GetCandidateMac(
        std::size_t index,
        Types::MacAddress& macAddress) const noexcept;


    void SelectReceiver(
        const Types::MacAddress& macAddress) noexcept;


    [[nodiscard]]
    bool IsInitialized() const noexcept;


    //=========================================================================
    // Estado
    //=========================================================================

    std::size_t m_receiverCandidate = 0;

    Types::MacAddress m_customReceiverMac{};

    bool m_hasCustomReceiverMac = false;


    Types::MacAddress m_selectedReceiver{};

    bool m_receiverSelected = false;


    volatile bool m_transmissionPending = false;

    volatile bool m_transmissionResultAvailable = false;

    volatile bool m_lastTransmissionSuccessful = false;


    bool m_initialized = false;


    Protocol::VehicleStatus m_lastVehicleStatus{};

    bool m_vehicleStatusAvailable = false;


    static ESPNowHandler* s_instance;
};

}

#endif