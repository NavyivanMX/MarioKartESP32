/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowReceiver.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Receptor ESP-NOW.
 * Recibe paquetes provenientes del Transmitter y los convierte a
 * DriverCommand utilizando el protocolo compartido.
 ******************************************************************************/

#ifndef MK_RECEIVER_ESPNOWRECEIVER_H
#define MK_RECEIVER_ESPNOWRECEIVER_H

//=============================================================================
// Includes
//=============================================================================

#include <cstdint>

#include <WiFi.h>
#include <esp_now.h>

#include <MKShared.h>

namespace MK
{

class ESPNowReceiver final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    [[nodiscard]]
    bool Begin();

    //=========================================================================
    // Recepción
    //=========================================================================

    [[nodiscard]]
    bool Receive(
        Protocol::DriverCommand& command);

private:

    //=========================================================================
    // Callback
    //=========================================================================

    static void OnReceive(
        const esp_now_recv_info* info,
        const std::uint8_t* data,
        int length);

private:

    static volatile bool m_packetAvailable;

    static std::uint8_t m_packet[
        Protocol::PacketSize<
            Protocol::DriverCommand>()];
};

} // namespace MK

#endif