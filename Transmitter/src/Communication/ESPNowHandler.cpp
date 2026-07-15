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

void OnDataSent(
    const wifi_tx_info_t* tx_info,
    esp_now_send_status_t status)
{
    (void)tx_info;

#if ESPNOW_DEBUG

    Serial.print("[ESP-NOW] Send callback -> ");

    if (status == ESP_NOW_SEND_SUCCESS)
    {
        Serial.println("SUCCESS");
    }
    else
    {
        Serial.print("FAILED (");

        switch (status)
        {
            case ESP_NOW_SEND_SUCCESS:
                Serial.print("SUCCESS");
                break;

            case ESP_NOW_SEND_FAIL:
                Serial.print("FAIL");
                break;

            default:
                Serial.print(static_cast<int>(status));
                break;
        }

        Serial.println(")");

    }

#endif
}

} // namespace

namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

bool ESPNowHandler::Begin()
{
    Serial.print("DriverCommand size TX: ");
Serial.println(sizeof(Protocol::DriverCommand));

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
    const Protocol::DriverCommand& command) noexcept
{
    if (!IsInitialized())
    {
#if ESPNOW_DEBUG
        Serial.println("[ESP-NOW] ERROR: Not initialized.");
#endif
        return false;
    }

    const esp_err_t result =
        esp_now_send(
            m_peer.peer_addr,
            reinterpret_cast<const uint8_t*>(&command),
            sizeof(command));

#if ESPNOW_DEBUG

    Serial.print("[ESP-NOW] esp_now_send() -> ");
    Serial.println(result);

#endif

    return IsSuccess(result);
}

//=============================================================================
// Inicialización
//=============================================================================

bool ESPNowHandler::InitializeWiFi() noexcept
{
    WiFi.mode(WIFI_STA);

    WiFi.disconnect();

    // Fuerza el canal configurado para ESP-NOW.
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
    const esp_err_t result = esp_now_init();

    Serial.print("DriverCommand size TX: ");
Serial.println(sizeof(Protocol::DriverCommand));

#if ESPNOW_DEBUG

    Serial.print("[ESP-NOW] esp_now_init() -> ");
    Serial.println(result);

#endif

    if (result != ESP_OK)
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