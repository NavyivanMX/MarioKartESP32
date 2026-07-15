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
    return (result == ESP_OK);
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
    Serial.print("DriverCommand size RX: ");
Serial.println(sizeof(Protocol::DriverCommand));
    if (!InitializeWiFi())
    {
#if ESPNOW_DEBUG
        Serial.println(F("[ESP-NOW] ERROR: InitializeWiFi()"));
#endif
        return false;
    }

    if (!InitializeESPNow())
    {
#if ESPNOW_DEBUG
        Serial.println(F("[ESP-NOW] ERROR: InitializeESPNow()"));
#endif
        return false;
    }

#if ESPNOW_DEBUG

    Serial.println();
    Serial.println(F("========== ESP-NOW RECEIVER =========="));

    Serial.print(F("MAC      : "));
    Serial.println(WiFi.macAddress());

    Serial.print(F("Canal    : "));
    Serial.println(WiFi.channel());

    Serial.println(F("ESP-NOW inicializado."));
    Serial.println(F("Esperando paquetes..."));
    Serial.println(F("======================================"));

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
    const esp_err_t result = esp_now_init();

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
    int len)
{

    Serial.println();
    Serial.println(F("********** PAQUETE RECIBIDO **********"));
    //----------------------------------------------------------
    // Validar longitud del paquete.
    //----------------------------------------------------------

    if (len != sizeof(Protocol::DriverCommand))
    {
#if ESPNOW_DEBUG
        Serial.print(F("[ESP-NOW] Invalid packet size: "));
        Serial.println(len);
#endif
        return;
    }

    //----------------------------------------------------------
    // Validar origen.
    //----------------------------------------------------------

    if (std::memcmp(
            info->src_addr,
            ReceiverConfig::TransmitterMacAddress.data(),
            ReceiverConfig::TransmitterMacAddress.size()) != 0)
    {
#if ESPNOW_DEBUG
        Serial.println(F("[ESP-NOW] Unknown transmitter."));
#endif
        return;
    }

    //----------------------------------------------------------
    // Copiar comando recibido.
    //----------------------------------------------------------

    std::memcpy(
        &m_command,
        data,
        sizeof(Protocol::DriverCommand));

    m_hasNewCommand = true;

#if ESPNOW_DEBUG

    Serial.println();
    Serial.println(F("========== ESP-NOW =========="));

    Serial.print(F("Origen : "));

    for (int i = 0; i < 6; ++i)
    {
        Serial.printf("%02X", info->src_addr[i]);

        if (i < 5)
        {
            Serial.print(':');
        }
    }

    Serial.println();

    Serial.print(F("Bytes  : "));
    Serial.println(len);

    Serial.println(F("============================="));

#endif
}

} // namespace MK