/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowReceiver.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del receptor ESP-NOW.
 * Se encarga de inicializar la comunicación inalámbrica, recibir los
 * paquetes enviados por el transmisor y almacenarlos para su posterior
 * procesamiento por el ReceiverController.
 ******************************************************************************/

#include "src/Communication/ESPNowReceiver.h"

#include <Arduino.h>

namespace MK
{

//---------------------------------------------------------------------------
// Variables estáticas
//---------------------------------------------------------------------------

Protocol::DriverCommand ESPNowReceiver::m_command{};

bool ESPNowReceiver::m_hasNewCommand = false;

//---------------------------------------------------------------------------
// Inicialización
//---------------------------------------------------------------------------

void ESPNowReceiver::Initialize() noexcept
{
    WiFi.mode(WIFI_STA);

    Serial.println();
    Serial.println(F("========== ESP-NOW RECEIVER =========="));

    Serial.print(F("MAC      : "));
    Serial.println(WiFi.macAddress());

    Serial.print(F("Canal    : "));
    Serial.println(WiFi.channel());

    if (esp_now_init() != ESP_OK)
    {
        Serial.println(F("[ERROR] esp_now_init()"));
        return;
    }

    esp_now_register_recv_cb(OnReceive);

    Serial.println(F("ESP-NOW inicializado."));
    Serial.println(F("Esperando paquetes..."));
    Serial.println(F("======================================"));
}

//---------------------------------------------------------------------------
// Consulta de estado
//---------------------------------------------------------------------------

bool ESPNowReceiver::HasNewCommand() const noexcept
{
    return m_hasNewCommand;
}

//---------------------------------------------------------------------------
// Obtiene el último comando recibido
//---------------------------------------------------------------------------

const Protocol::DriverCommand&
ESPNowReceiver::GetCommand() const noexcept
{
    m_hasNewCommand = false;

    return m_command;
}

//---------------------------------------------------------------------------
// Callback de recepción ESP-NOW
//---------------------------------------------------------------------------

void ESPNowReceiver::OnReceive(
    const esp_now_recv_info_t* info,
    const uint8_t* data,
    int len)
{
    if (len != sizeof(Protocol::DriverCommand))
    {
        Serial.println(F("[ESP-NOW] Tamaño de paquete inválido."));
        return;
    }

    memcpy(
        &m_command,
        data,
        sizeof(Protocol::DriverCommand));

    m_hasNewCommand = true;

#ifdef MK_DEBUG

    Serial.println();
    Serial.println(F("========== ESP-NOW =========="));

    Serial.print(F("MAC origen : "));

    for (int i = 0; i < 6; ++i)
    {
        Serial.printf("%02X", info->src_addr[i]);

        if (i < 5)
        {
            Serial.print(":");
        }
    }

    Serial.println();

    Serial.print(F("Longitud   : "));
    Serial.println(len);

    Serial.println(F("============================="));

#endif
}

} // namespace MK