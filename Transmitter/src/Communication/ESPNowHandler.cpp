/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowHandler.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del manejador de comunicación ESP-NOW.
 ******************************************************************************/

#include "ESPNowHandler.h"

#include <cstring>

#include <esp_now.h>
#include <esp_wifi.h>

#include <MKShared.h>

#include "src/Config/TransmitterConfig.h"

//=============================================================================
// DEBUG
// Cambiar a 0 para desactivar todos los mensajes de diagnóstico.
//=============================================================================

#define ESPNOW_DEBUG 1

namespace
{

[[nodiscard]]
bool IsSuccess(const esp_err_t result) noexcept
{
    return result == ESP_OK;
}

[[nodiscard]]
esp_now_peer_info_t CreatePeerInfo()
{
    esp_now_peer_info_t peer{};

    std::memcpy(
        peer.peer_addr,
        MK::TransmitterConfig::ReceiverMacAddress.data(),
        MK::TransmitterConfig::ReceiverMacAddress.size());

    peer.channel = MK::RadioConfig::Channel;
    peer.encrypt = MK::RadioConfig::Encryption;

    return peer;
}

// void OnDataSent(    const wifi_tx_info_t* tx_info,    esp_now_send_status_t status)
// {
//     (void)tx_info;

//     #if ESPNOW_DEBUG

//         if (status == ESP_NOW_SEND_SUCCESS)
//         {
//             Serial.println(
//                 "[ESP-NOW] Packet transmitted successfully.");
//         }
//         else
//         {
//             Serial.println(
//                 "[ESP-NOW] Packet transmission FAILED.");
//         }

//     #endif
// }

void OnDataSent(
    const wifi_tx_info_t* tx_info,
    esp_now_send_status_t status)
{
    (void)tx_info;

    Serial.print("[ESP-NOW] Callback Status: ");

    if (status == ESP_NOW_SEND_SUCCESS)
    {
        Serial.println("SUCCESS");
    }
    else
    {
        Serial.print("FAILED (");
        Serial.print(static_cast<int>(status));
        Serial.println(")");
    }
}

} // namespace

namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

bool ESPNowHandler::Begin()
{
#if ESPNOW_DEBUG

    Serial.println();
    Serial.println("========== MK Protocol ==========");

    Serial.print("Protocol Version : ");
    Serial.println(
        Protocol::DriverCommandSerializer::Version);

    Serial.print("Packet Size      : ");
    Serial.println(
        Protocol::DriverCommandSerializer::PacketSize);

    Serial.println("=================================");
    Serial.println();

#endif

    if (m_initialized)
    {
        return true;
    }

    if (!InitializeWiFi())
    {
#if ESPNOW_DEBUG
        Serial.println("[ESP-NOW] ERROR: InitializeWiFi()");
#endif
        return false;
    }

    if (!InitializeESPNow())
    {
#if ESPNOW_DEBUG
        Serial.println("[ESP-NOW] ERROR: InitializeESPNow()");
#endif
        return false;
    }

    m_peer = CreatePeerInfo();

    if (!RegisterPeer())
    {
#if ESPNOW_DEBUG
        Serial.println("[ESP-NOW] ERROR: RegisterPeer()");
#endif

        esp_now_deinit();

        return false;
    }

    m_initialized = true;

#if ESPNOW_DEBUG

    Serial.println("[ESP-NOW] Initialization completed.");

#endif

    return true;
}

void ESPNowHandler::End() noexcept
{
    if (!m_initialized)
    {
        return;
    }

    esp_now_deinit();

    m_initialized = false;
}

//=============================================================================
// Comunicación
//=============================================================================

bool ESPNowHandler::Send(
    const std::uint8_t* packet,
    std::size_t length) noexcept
{
    if (!IsInitialized())
    {
        return false;
    }

    if ((packet == nullptr) ||
        (length == 0))
    {
        return false;
    }

#if ESPNOW_DEBUG

    Serial.print("[ESP-NOW] TX (");
    Serial.print(length);
    Serial.print(" bytes): ");

    for (std::size_t i = 0; i < length; ++i)
    {
        if (packet[i] < 16)
        {
            Serial.print('0');
        }

        Serial.print(packet[i], HEX);
        Serial.print(' ');
    }

    Serial.println();

#endif

    const auto result =
        esp_now_send(
            m_peer.peer_addr,
            packet,
            length);

    return IsSuccess(result);
}

//=============================================================================
// Inicialización
//=============================================================================

bool ESPNowHandler::InitializeWiFi() noexcept
{
    WiFi.mode(WIFI_STA);

    WiFi.disconnect();

    esp_wifi_set_channel(
        MK::RadioConfig::Channel,
        WIFI_SECOND_CHAN_NONE);

#if ESPNOW_DEBUG

    Serial.println();
    Serial.println("========== ESP-NOW ==========");

    Serial.print("Mode    : ");
    Serial.println(WiFi.getMode());

    Serial.print("MAC     : ");
    Serial.println(WiFi.macAddress());

    Serial.print("Channel : ");
    Serial.println(WiFi.channel());

    Serial.println("=============================");

#endif

    return (WiFi.getMode() == WIFI_STA);
}

bool ESPNowHandler::InitializeESPNow() noexcept
{
    const esp_err_t result =
        esp_now_init();

#if ESPNOW_DEBUG

    Serial.print("[ESP-NOW] esp_now_init() -> ");
    Serial.println(result);

#endif

    if (!IsSuccess(result))
    {
        return false;
    }

    esp_now_register_send_cb(OnDataSent);

    return true;
}

bool ESPNowHandler::RegisterPeer() noexcept
{
    const esp_err_t result =
        esp_now_add_peer(&m_peer);

#if ESPNOW_DEBUG

    Serial.print("[ESP-NOW] esp_now_add_peer() -> ");
    Serial.println(result);

#endif

    return IsSuccess(result);
}

//=============================================================================
// Estado
//=============================================================================

bool ESPNowHandler::IsInitialized() const noexcept
{
    return m_initialized;
}

} // namespace MK