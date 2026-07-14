/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowReceiver.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del receptor ESP-NOW.
 ******************************************************************************/

#include "src/Communication/ESPNowReceiver.h"

#include <cstring>

namespace MK
{

Protocol::DriverCommand ESPNowReceiver::s_receivedCommand{};

volatile bool ESPNowReceiver::s_commandAvailable = false;

void ESPNowReceiver::Initialize() noexcept
{
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK)
    {
        return;
    }

    esp_now_register_recv_cb(OnReceive);
}

bool ESPNowReceiver::Receive(
    Protocol::DriverCommand& command) noexcept
{
    if (!s_commandAvailable)
    {
        return false;
    }

    command = s_receivedCommand;

    s_commandAvailable = false;

    return true;
}

void ESPNowReceiver::OnReceive(
    const esp_now_recv_info_t* info,
    const uint8_t* data,
    int len)
{
    (void)info;

    if (len != sizeof(Protocol::DriverCommand))
    {
        return;
    }

    memcpy(
        &s_receivedCommand,
        data,
        sizeof(Protocol::DriverCommand));

    s_commandAvailable = true;
}

}