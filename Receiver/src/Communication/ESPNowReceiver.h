/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowReceiver.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Encapsula la inicialización y comunicación mediante ESP-NOW.
 ******************************************************************************/

#pragma once

#include <esp_now.h>
#include <WiFi.h>

#include <MKShared.h>

namespace MK
{

class ESPNowReceiver
{
public:

    void Initialize() noexcept;

    bool HasNewCommand() const noexcept;

    const Protocol::DriverCommand&
    GetCommand() const noexcept;

private:

    static void OnReceive(
        const esp_now_recv_info_t* info,
        const uint8_t* data,
        int len);

private:

    static Protocol::DriverCommand m_command;

    static bool m_hasNewCommand;
};

}