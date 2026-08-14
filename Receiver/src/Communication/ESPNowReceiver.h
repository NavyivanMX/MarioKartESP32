/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowReceiver.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Receptor ESP-NOW.
 *
 * Recibe paquetes provenientes del Transmitter y los convierte a
 * DriverCommand utilizando el protocolo compartido.
 *
 * También permite enviar VehicleStatus de regreso al último
 * Transmitter que haya enviado un comando válido.
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

    //=========================================================================
    // Transmisión
    //=========================================================================

    /**
     * @brief Envía VehicleStatus al último Transmitter válido recibido.
     */
    [[nodiscard]]
    bool SendVehicleStatus(
        const Protocol::VehicleStatus& status) noexcept;

private:

    //=========================================================================
    // Callback ESP-NOW
    //=========================================================================

    static void OnReceive(
        const esp_now_recv_info* info,
        const std::uint8_t* data,
        int length);

private:

    //=========================================================================
    // RX
    //=========================================================================

    static volatile bool m_packetAvailable;

    static std::uint8_t m_packet[
        Protocol::PacketSize<
            Protocol::DriverCommand>()];

    //=========================================================================
    // Transmitter actual
    //=========================================================================

    Types::MacAddress m_lastTransmitterMac{};

    bool m_hasLastTransmitter = false;

    //=========================================================================
    // Instancia activa
    //=========================================================================
    //
    // El callback de ESP-NOW es estático, por lo que utilizamos esta
    // referencia para acceder a los datos de la instancia activa.
    //

    static ESPNowReceiver* s_instance;
};

} // namespace MK

#endif // MK_RECEIVER_ESPNOWRECEIVER_H