/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowReceiver.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Encapsula la recepción de paquetes mediante ESP-NOW.
 ******************************************************************************/

#pragma once

#include <WiFi.h>
#include <esp_now.h>

#include <MKShared.h>

namespace MK
{

class ESPNowReceiver
{
public:

    void Initialize() noexcept;

    bool Receive(
        Protocol::DriverCommand& command) noexcept;

private:

    static void OnReceive(
        const esp_now_recv_info_t* info,
        const uint8_t* data,
        int len);

private:

    static Protocol::DriverCommand s_receivedCommand;

    static volatile bool s_commandAvailable;
};

}