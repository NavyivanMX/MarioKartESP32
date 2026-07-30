/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowReceiver.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del receptor ESP-NOW.
 ******************************************************************************/

#include "ESPNowReceiver.h"

#include <cstring>

namespace MK
{

//=============================================================================
// Variables estáticas
//=============================================================================

volatile bool
ESPNowReceiver::m_packetAvailable = false;

std::uint8_t
ESPNowReceiver::m_packet[
    Protocol::DriverCommandSerializer::PacketSize];

//=============================================================================
// Inicialización
//=============================================================================

bool ESPNowReceiver::Begin()
{
    //-------------------------------------------------------------
    // Modo estación
    //-------------------------------------------------------------

    WiFi.mode(WIFI_STA);

    //-------------------------------------------------------------
    // Inicializar ESP-NOW
    //-------------------------------------------------------------

    if (esp_now_init() != ESP_OK)
    {
        return false;
    }

    //-------------------------------------------------------------
    // Registrar callback
    //-------------------------------------------------------------

    esp_now_register_recv_cb(
        ESPNowReceiver::OnReceive);

    return true;
}

//=============================================================================
// Recepción
//=============================================================================

bool ESPNowReceiver::Receive(
    Protocol::DriverCommand& command)
{
    //-------------------------------------------------------------
    // ¿Hay paquete nuevo?
    //-------------------------------------------------------------

    if (!m_packetAvailable)
    {
        return false;
    }

    //-------------------------------------------------------------
    // Consumir paquete
    //-------------------------------------------------------------

    m_packetAvailable = false;

    //-------------------------------------------------------------
    // Deserializar
    //-------------------------------------------------------------

    return Protocol::DriverCommandSerializer::
        Deserialize(
            m_packet,
            command);
}

//=============================================================================
// Callback ESP-NOW
//=============================================================================

void ESPNowReceiver::OnReceive(
    const esp_now_recv_info* info,
    const std::uint8_t* data,
    int length)
{
    (void)info;

    //-------------------------------------------------------------
    // Validar longitud
    //-------------------------------------------------------------

    constexpr std::size_t PacketSize =
        Protocol::DriverCommandSerializer::PacketSize;

    if (length != PacketSize)
    {
        return;
    }

    //-------------------------------------------------------------
    // Copiar paquete recibido
    //-------------------------------------------------------------

    std::memcpy(
        m_packet,
        data,
        PacketSize);

    //-------------------------------------------------------------
    // Indicar paquete disponible
    //-------------------------------------------------------------

    m_packetAvailable = true;
}

} // namespace MK