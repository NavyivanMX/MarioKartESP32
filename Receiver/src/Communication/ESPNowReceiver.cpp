/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowReceiver.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del receptor ESP-NOW.
 ******************************************************************************/

#include "src/Communication/ESPNowReceiver.h"

#include <Arduino.h>
#include <WiFi.h>

#include <cstring>

#include <esp_now.h>
#include <esp_wifi.h>

#include <Config/RadioConfig.h>
#include "src/Config/ReceiverConfig.h"

namespace
{

//=============================================================================
// Configuración
//=============================================================================

#define ESPNOW_DEBUG 1

//=============================================================================
// Utilidades
//=============================================================================

[[nodiscard]]
bool IsSuccess(const esp_err_t result) noexcept
{
    return result == ESP_OK;
}

} // namespace

namespace MK
{

//=============================================================================
// Variables estáticas
//=============================================================================

Protocol::DriverCommand ESPNowReceiver::m_command{};

bool ESPNowReceiver::m_hasNewCommand = false;

//=============================================================================
// Ciclo de vida
//=============================================================================

bool ESPNowReceiver::Begin() noexcept
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

#if ESPNOW_DEBUG

    Serial.println();
    Serial.println("========== ESP-NOW RECEIVER ==========");

    Serial.print("MAC      : ");
    Serial.println(WiFi.macAddress());

    Serial.print("Channel  : ");
    Serial.println(WiFi.channel());

    Serial.println("ESP-NOW initialized.");
    Serial.println("Waiting packets...");
    Serial.println("======================================");

#endif

    return true;
}

//=============================================================================
// Inicialización
//=============================================================================

bool ESPNowReceiver::InitializeWiFi() noexcept
{
    WiFi.mode(WIFI_STA);

    WiFi.disconnect();

    esp_wifi_set_channel(
        RadioConfig::Channel,
        WIFI_SECOND_CHAN_NONE);

    return (WiFi.getMode() == WIFI_STA);
}

bool ESPNowReceiver::InitializeESPNow() noexcept
{
    const auto result =
        esp_now_init();

    if (!IsSuccess(result))
    {
        return false;
    }

    esp_now_register_recv_cb(OnReceive);

    return true;
}

//=============================================================================
// Comunicación
//=============================================================================

bool ESPNowReceiver::HasNewCommand() const noexcept
{
    return m_hasNewCommand;
}

const Protocol::DriverCommand&
ESPNowReceiver::GetCommand() noexcept
{
    m_hasNewCommand = false;

    return m_command;
}

//=============================================================================
// Callback ESP-NOW
//=============================================================================

void ESPNowReceiver::OnReceive(
    const esp_now_recv_info_t* info,
    const uint8_t* data,
    int length)
{
    //----------------------------------------------------------
    // Validar transmisor
    //----------------------------------------------------------

    if (std::memcmp(
            info->src_addr,
            ReceiverConfig::TransmitterMacAddress.data(),
            ReceiverConfig::TransmitterMacAddress.size()) != 0)
    {
        #if ESPNOW_DEBUG
                Serial.println("[ESP-NOW] Unknown transmitter.");
        #endif
            return;
    }

    //----------------------------------------------------------
    // Validar tamaño
    //----------------------------------------------------------

    if (length !=
        Protocol::DriverCommandSerializer::PacketSize)
    {
#if ESPNOW_DEBUG

        Serial.print("[ESP-NOW] Invalid packet size: ");
        Serial.println(length);

#endif
        return;
    }

#if ESPNOW_DEBUG

    Serial.print("[ESP-NOW] RX (");
    Serial.print(length);
    Serial.print(" bytes): ");

    for (int i = 0; i < length; ++i)
    {
        if (data[i] < 16)
        {
            Serial.print('0');
        }

        Serial.print(data[i], HEX);
        Serial.print(' ');
    }

    Serial.println();

#endif

    //----------------------------------------------------------
    // Deserializar
    //----------------------------------------------------------

    Protocol::DriverCommand command{};

    if (!Protocol::DriverCommandSerializer::Deserialize(
            data,
            command))
    {
#if ESPNOW_DEBUG

        Serial.println(
            "[ESP-NOW] Invalid protocol packet.");

#endif
        return;
    }

    //----------------------------------------------------------
    // Actualizar comando
    //----------------------------------------------------------

    m_command = command;

    m_hasNewCommand = true;

#if ESPNOW_DEBUG

    Serial.println("[ESP-NOW] DriverCommand updated.");

#endif
}

} // namespace MK