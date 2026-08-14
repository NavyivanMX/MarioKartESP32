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

#include "src/Config/ReceiverConfig.h"

namespace MK
{

//=============================================================================
// Variables estáticas
//=============================================================================

volatile bool
ESPNowReceiver::m_packetAvailable =
    false;


std::uint8_t
ESPNowReceiver::m_packet[
    Protocol::PacketSize<
        Protocol::DriverCommand>()];


ESPNowReceiver*
ESPNowReceiver::s_instance =
    nullptr;


//=============================================================================
// Inicialización
//=============================================================================

bool ESPNowReceiver::Begin()
{
    //---------------------------------------------------------------------
    // Evitar inicialización duplicada
    //---------------------------------------------------------------------

    if (s_instance == this)
    {
        return true;
    }

    //---------------------------------------------------------------------
    // Registrar instancia
    //---------------------------------------------------------------------

    s_instance =
        this;

    //---------------------------------------------------------------------
    // Modo estación
    //---------------------------------------------------------------------

    WiFi.mode(WIFI_STA);

    //---------------------------------------------------------------------
    // Inicializar ESP-NOW
    //---------------------------------------------------------------------

    if (esp_now_init() != ESP_OK)
    {
        s_instance =
            nullptr;

        return false;
    }

    //---------------------------------------------------------------------
    // Registrar callback
    //---------------------------------------------------------------------

    esp_now_register_recv_cb(
        ESPNowReceiver::OnReceive);

    //---------------------------------------------------------------------
    // Registrar Transmitters autorizados
    //---------------------------------------------------------------------

    for (const auto& mac :
         ReceiverConfig::AuthorizedTransmitters)
    {
        esp_now_peer_info_t peerInfo{};

        std::memcpy(
            peerInfo.peer_addr,
            mac.data(),
            mac.size());

        peerInfo.channel =
            0;

        peerInfo.encrypt =
            false;

        //-----------------------------------------------------------------
        // Si ya existe, no volver a registrarlo
        //-----------------------------------------------------------------

        if (esp_now_is_peer_exist(
                peerInfo.peer_addr))
        {
            continue;
        }

        //-----------------------------------------------------------------
        // Agregar peer
        //-----------------------------------------------------------------

        if (esp_now_add_peer(
                &peerInfo) != ESP_OK)
        {
            esp_now_deinit();

            s_instance =
                nullptr;

            return false;
        }
    }

    //---------------------------------------------------------------------
    // Estado inicial
    //---------------------------------------------------------------------

    m_packetAvailable =
        false;

    m_hasLastTransmitter =
        false;

    return true;
}


//=============================================================================
// Recepción
//=============================================================================

bool ESPNowReceiver::Receive(
    Protocol::DriverCommand& command)
{
    //---------------------------------------------------------------------
    // ¿Hay paquete nuevo?
    //---------------------------------------------------------------------

    if (!m_packetAvailable)
    {
        return false;
    }

    //---------------------------------------------------------------------
    // Consumir paquete
    //---------------------------------------------------------------------

    m_packetAvailable =
        false;

    //---------------------------------------------------------------------
    // Deserializar
    //---------------------------------------------------------------------

    Protocol::Packet<
        Protocol::DriverCommand> packet{};

    if (!Protocol::PacketSerializer::Deserialize(
            m_packet,
            sizeof(m_packet),
            packet))
    {
        return false;
    }

    //---------------------------------------------------------------------
    // Validar tipo
    //---------------------------------------------------------------------

    if (packet.header.type !=
        Protocol::PacketType::DriverCommand)
    {
        return false;
    }

    //---------------------------------------------------------------------
    // Validar tamaño del payload
    //---------------------------------------------------------------------

    if (packet.header.payloadSize !=
        sizeof(Protocol::DriverCommand))
    {
        return false;
    }

    //---------------------------------------------------------------------
    // Copiar payload
    //---------------------------------------------------------------------

    command =
        packet.payload;

    return true;
}


//=============================================================================
// Transmisión
//=============================================================================

bool ESPNowReceiver::SendVehicleStatus(
    const Protocol::VehicleStatus& status) noexcept
{
    //---------------------------------------------------------------------
    // Todavía no conocemos ningún Transmitter
    //---------------------------------------------------------------------

    if (!m_hasLastTransmitter)
    {
        return false;
    }

    //---------------------------------------------------------------------
    // Construir Packet
    //---------------------------------------------------------------------

    Protocol::Packet<
        Protocol::VehicleStatus> packet{};

    packet.header.type =
        Protocol::PacketType::VehicleStatus;

    packet.header.payloadSize =
        sizeof(Protocol::VehicleStatus);

    packet.payload =
        status;

    //---------------------------------------------------------------------
    // Buffer
    //---------------------------------------------------------------------

    std::uint8_t buffer[
        Protocol::PacketSize<
            Protocol::VehicleStatus>()];

    //---------------------------------------------------------------------
    // Serializar
    //---------------------------------------------------------------------

    if (!Protocol::PacketSerializer::Serialize(
            packet,
            buffer,
            sizeof(buffer)))
    {
        return false;
    }

    //---------------------------------------------------------------------
    // Enviar al último Transmitter
    //---------------------------------------------------------------------

    const esp_err_t result =
        esp_now_send(
            m_lastTransmitterMac.data(),
            buffer,
            sizeof(buffer));

    return result == ESP_OK;
}


//=============================================================================
// Callback ESP-NOW
//=============================================================================

void ESPNowReceiver::OnReceive(
    const esp_now_recv_info* info,
    const std::uint8_t* data,
    int length)
{
    //---------------------------------------------------------------------
    // Verificar instancia
    //---------------------------------------------------------------------

    if (s_instance == nullptr)
    {
        return;
    }

    //---------------------------------------------------------------------
    // Verificar información del paquete
    //---------------------------------------------------------------------

    if (info == nullptr)
    {
        return;
    }

    //---------------------------------------------------------------------
    // Verificar datos
    //---------------------------------------------------------------------

    if (data == nullptr ||
        length <= 0)
    {
        return;
    }

    //---------------------------------------------------------------------
    // Obtener MAC del transmisor
    //---------------------------------------------------------------------

    Types::MacAddress senderMac{};

    std::memcpy(
        senderMac.data(),
        info->src_addr,
        senderMac.size());

    //---------------------------------------------------------------------
    // Validar MAC si está habilitado
    //---------------------------------------------------------------------

    if (ReceiverConfig::ValidateTransmitterMac)
    {
        if (!ReceiverConfig::IsAuthorizedTransmitter(
                senderMac))
        {
            return;
        }
    }

    //---------------------------------------------------------------------
    // Tamaño esperado
    //---------------------------------------------------------------------

    constexpr std::size_t PacketSize =
        Protocol::PacketSize<
            Protocol::DriverCommand>();

    if (static_cast<std::size_t>(length) !=
        PacketSize)
    {
        return;
    }

    //---------------------------------------------------------------------
    // Guardar MAC del último Transmitter válido
    //---------------------------------------------------------------------

    s_instance->m_lastTransmitterMac =
        senderMac;

    s_instance->m_hasLastTransmitter =
        true;

    //---------------------------------------------------------------------
    // Copiar paquete
    //---------------------------------------------------------------------

    std::memcpy(
        m_packet,
        data,
        PacketSize);

    //---------------------------------------------------------------------
    // Indicar paquete disponible
    //---------------------------------------------------------------------

    m_packetAvailable =
        true;
}

} // namespace MK