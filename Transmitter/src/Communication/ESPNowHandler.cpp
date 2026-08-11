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
//=============================================================================

#define ESPNOW_DEBUG 0

namespace
{

//=============================================================================
// Utilidades
//=============================================================================

[[nodiscard]]
bool IsSuccess(
    const esp_err_t result) noexcept
{
    return result == ESP_OK;
}

//=============================================================================
// Peer
//=============================================================================

[[nodiscard]]
esp_now_peer_info_t CreatePeerInfo()
{
    esp_now_peer_info_t peer{};

    std::memcpy(
        peer.peer_addr,
        MK::TransmitterConfig::ReceiverMacAddress.data(),
        MK::TransmitterConfig::ReceiverMacAddress.size());

    peer.channel =
        MK::RadioConfig::Channel;

    peer.encrypt =
        MK::RadioConfig::Encryption;

    return peer;
}

//=============================================================================
// TX Callback
//=============================================================================

void OnDataSent(
    const wifi_tx_info_t* tx_info,
    esp_now_send_status_t status)
{
    (void)tx_info;
    (void)status;
}

//=============================================================================
// RX Callback
//=============================================================================

void OnDataReceive(
    const esp_now_recv_info_t* info,
    const std::uint8_t* data,
    int length)
{
    (void)info;

    if (data == nullptr)
    {
        return;
    }

    if (length <= 0)
    {
        return;
    }

    //=====================================================================
    // Validar tamaño mínimo del paquete
    //=====================================================================

    if (static_cast<std::size_t>(length) <
        MK::Protocol::PacketSize<
            MK::Protocol::VehicleStatus>())
    {
        return;
    }

    //=====================================================================
    // El callback no debería hacer lógica.
    //
    // Guardamos el paquete para procesarlo posteriormente desde
    // ReceiveVehicleStatus().
    //=====================================================================

    MK::Protocol::Packet<
        MK::Protocol::VehicleStatus> packet{};

    if (!MK::Protocol::PacketSerializer::Deserialize(
            data,
            static_cast<std::size_t>(length),
            packet))
    {
        return;
    }

    //=====================================================================
    // Validar tipo de paquete
    //=====================================================================

    if (packet.header.type !=
        MK::Protocol::PacketType::VehicleStatus)
    {
        return;
    }

    //=====================================================================
    // Guardar estado
    //=====================================================================

    // Esta parte será accedida por el loop principal.
    //
    // Para esta primera implementación mantenemos el mecanismo simple:
    // copiar el último estado recibido y marcarlo disponible.

    // Nota:
    // El callback no puede acceder directamente a la instancia
    // ESPNowHandler porque esp_now_register_recv_cb() utiliza una
    // función global/static.
}

} // namespace

namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

bool ESPNowHandler::Begin()
{
#if ESPNOW_DEBUG

    Serial.println();
    Serial.println("========== MK Protocol ==========");

    Serial.print("DriverCommand Packet Size: ");
    Serial.println(
        Protocol::PacketSize<
            Protocol::DriverCommand>());

    Serial.print("VehicleStatus Packet Size: ");
    Serial.println(
        Protocol::PacketSize<
            Protocol::VehicleStatus>());

    Serial.println("=================================");
    Serial.println();

#endif

    //---------------------------------------------------------------------
    // Evitar inicialización duplicada
    //---------------------------------------------------------------------

    if (m_initialized)
    {
        return true;
    }

    //---------------------------------------------------------------------
    // WiFi
    //---------------------------------------------------------------------

    if (!InitializeWiFi())
    {
#if ESPNOW_DEBUG
        Serial.println(
            "[ESP-NOW] ERROR: InitializeWiFi()");
#endif

        return false;
    }

    //---------------------------------------------------------------------
    // ESP-NOW
    //---------------------------------------------------------------------

    if (!InitializeESPNow())
    {
#if ESPNOW_DEBUG
        Serial.println(
            "[ESP-NOW] ERROR: InitializeESPNow()");
#endif

        return false;
    }

    //---------------------------------------------------------------------
    // Peer
    //---------------------------------------------------------------------

    m_peer =
        CreatePeerInfo();

    if (!RegisterPeer())
    {
#if ESPNOW_DEBUG
        Serial.println(
            "[ESP-NOW] ERROR: RegisterPeer()");
#endif

        esp_now_deinit();

        return false;
    }

    //---------------------------------------------------------------------
    // Listo
    //---------------------------------------------------------------------

    m_initialized = true;

    m_vehicleStatusAvailable = false;

#if ESPNOW_DEBUG
    Serial.println(
        "[ESP-NOW] Initialization completed.");
#endif

    return true;
}

//=============================================================================
// End
//=============================================================================

void ESPNowHandler::End() noexcept
{
    if (!m_initialized)
    {
        return;
    }

    esp_now_deinit();

    m_initialized = false;
    m_vehicleStatusAvailable = false;
}

//=============================================================================
// TX
//=============================================================================

bool ESPNowHandler::Send(
    const std::uint8_t* packet,
    std::size_t length) noexcept
{
    if (!IsInitialized())
    {
        return false;
    }

    if (packet == nullptr ||
        length == 0)
    {
        return false;
    }

#if ESPNOW_DEBUG

    Serial.print(
        "[ESP-NOW] TX (");

    Serial.print(length);

    Serial.print(
        " bytes): ");

    for (std::size_t i = 0; i < length; ++i)
    {
        if (packet[i] < 16)
        {
            Serial.print('0');
        }

        Serial.print(
            packet[i],
            HEX);

        Serial.print(' ');
    }

    Serial.println();

#endif

    const esp_err_t result =
        esp_now_send(
            m_peer.peer_addr,
            packet,
            length);

    return IsSuccess(result);
}

//=============================================================================
// RX
//=============================================================================

bool ESPNowHandler::ReceiveVehicleStatus(
    Protocol::VehicleStatus& status) noexcept
{
    //---------------------------------------------------------------------
    // No hay estado pendiente
    //---------------------------------------------------------------------

    if (!m_vehicleStatusAvailable)
    {
        return false;
    }

    //---------------------------------------------------------------------
    // Copiar estado
    //---------------------------------------------------------------------

    status =
        m_lastVehicleStatus;

    //---------------------------------------------------------------------
    // Consumir evento
    //---------------------------------------------------------------------

    m_vehicleStatusAvailable =
        false;

    return true;
}

//=============================================================================
// Inicialización WiFi
//=============================================================================

bool ESPNowHandler::InitializeWiFi() noexcept
{
    WiFi.mode(WIFI_STA);

    WiFi.disconnect();

    esp_wifi_set_channel(
        MK::RadioConfig::Channel,
        WIFI_SECOND_CHAN_NONE);

#if ESPNOW_DEBUG

    Serial.println();
    Serial.println(
        "========== ESP-NOW ==========");

    Serial.print("Mode    : ");
    Serial.println(
        WiFi.getMode());

    Serial.print("MAC     : ");
    Serial.println(
        WiFi.macAddress());

    Serial.print("Channel : ");
    Serial.println(
        WiFi.channel());

    Serial.println(
        "=============================");

#endif

    return
        WiFi.getMode() ==
        WIFI_STA;
}

//=============================================================================
// Inicialización ESP-NOW
//=============================================================================

bool ESPNowHandler::InitializeESPNow() noexcept
{
    const esp_err_t result =
        esp_now_init();

#if ESPNOW_DEBUG

    Serial.print(
        "[ESP-NOW] esp_now_init() -> ");

    Serial.println(result);

#endif

    if (!IsSuccess(result))
    {
        return false;
    }

    //---------------------------------------------------------------------
    // TX callback
    //---------------------------------------------------------------------

    esp_now_register_send_cb(
        OnDataSent);

    //---------------------------------------------------------------------
    // RX callback
    //---------------------------------------------------------------------

    /*
     * Aquí registraremos posteriormente el callback asociado a la
     * instancia de ESPNowHandler.
     *
     * Para hacerlo correctamente necesitamos una pequeña capa de
     * asociación entre el callback estático de ESP-NOW y la instancia
     * m_espNowHandler.
     */

    return true;
}

//=============================================================================
// Peer
//=============================================================================

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