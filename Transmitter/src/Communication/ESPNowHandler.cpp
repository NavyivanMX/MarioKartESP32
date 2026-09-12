/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowHandler.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación de comunicación ESP-NOW para el Transmitter.
 *
 * Permite seleccionar automáticamente uno de dos receptores:
 *
 *   1. Kart
 *   2. Laboratorio
 *
 * El primer receptor disponible queda seleccionado durante toda la sesión.
 * Para cambiar de receptor es necesario reiniciar el Transmitter.
 ******************************************************************************/

#include "ESPNowHandler.h"

#include <cstring>

#include <esp_now.h>
#include <esp_wifi.h>

#include <MKShared.h>

#include "src/Config/TransmitterConfig.h"


namespace MK
{

//=============================================================================
// Instancia estática
//=============================================================================

ESPNowHandler* ESPNowHandler::s_instance = nullptr;


//=============================================================================
// Debug
//=============================================================================

#define ESPNOW_DEBUG 0


namespace
{

//=============================================================================
// Utilidades
//=============================================================================

bool IsSuccess(
    const esp_err_t result) noexcept
{
    return result == ESP_OK;
}


//-----------------------------------------------------------------------------
// Crea información de peer
//-----------------------------------------------------------------------------

esp_now_peer_info_t CreatePeerInfo(
    const Types::MacAddress& macAddress)
{
    esp_now_peer_info_t peer{};

    std::memcpy(
        peer.peer_addr,
        macAddress.data(),
        macAddress.size());

    peer.channel = MK::RadioConfig::Channel;
    peer.ifidx = WIFI_IF_STA;
    peer.encrypt = MK::RadioConfig::Encryption;

    return peer;
}

} // namespace


//=============================================================================
// Begin
//=============================================================================

bool ESPNowHandler::Begin()
{
    if (m_initialized)
    {
        return true;
    }

    s_instance = this;

    //-------------------------------------------------------------------------
    // WiFi
    //-------------------------------------------------------------------------

    if (!InitializeWiFi())
    {
        return false;
    }


    //-------------------------------------------------------------------------
    // ESP-NOW
    //-------------------------------------------------------------------------

    if (!InitializeESPNow())
    {
        return false;
    }


    //-------------------------------------------------------------------------
    // Registrar receptores
    //-------------------------------------------------------------------------

    if (!RegisterPeers())
    {
        return false;
    }


    //-------------------------------------------------------------------------
    // Estado inicial
    //-------------------------------------------------------------------------

    m_receiverSelected = false;
    m_receiverCandidate = KartReceiverIndex;

    m_transmissionPending = false;
    m_transmissionResultAvailable = false;
    m_lastTransmissionSuccessful = false;

    m_vehicleStatusAvailable = false;

    m_initialized = true;


#if ESPNOW_DEBUG
    Serial.println(
        "[ESP-NOW] Inicializado.");

    Serial.println(
        "[ESP-NOW] Buscando receptor...");
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

    esp_now_unregister_recv_cb();
    esp_now_unregister_send_cb();

    esp_now_deinit();

    m_initialized = false;

    if (s_instance == this)
    {
        s_instance = nullptr;
    }
}


//=============================================================================
// Send
//=============================================================================

bool ESPNowHandler::Send(
    const std::uint8_t* packet,
    const std::size_t length) noexcept
{
    if (!m_initialized)
    {
        return false;
    }

    if (packet == nullptr || length == 0)
    {
        return false;
    }


    //=========================================================================
    // Si todavía estamos esperando confirmación del candidato actual,
    // NO enviamos nada.
    //
    // Esto es importante:
    //
    // Nunca enviamos simultáneamente el mismo comando al Kart y al Laboratorio.
    //=========================================================================

    if (m_transmissionPending)
    {
        return false;
    }


    //=========================================================================
    // Procesar resultado de la transmisión anterior
    //=========================================================================

    if (!m_receiverSelected &&
        m_transmissionResultAvailable)
    {
        m_transmissionResultAvailable = false;


        if (m_lastTransmissionSuccessful)
        {
            //-----------------------------------------------------------------
            // El candidato actual respondió.
            //
            // Lo dejamos seleccionado para toda la sesión.
            //-----------------------------------------------------------------

            if (m_receiverCandidate == KartReceiverIndex)
            {
                SelectReceiver(
                    TransmitterConfig::ReceiverMacAddressKart);
            }
            else
            {
                SelectReceiver(
                    TransmitterConfig::ReceiverMacAddressLab);
            }
        }
        else
        {
            //-----------------------------------------------------------------
            // El candidato no respondió.
            //
            // Pasamos al siguiente.
            //-----------------------------------------------------------------

            if (m_receiverCandidate == KartReceiverIndex)
            {
                m_receiverCandidate = LabReceiverIndex;

#if ESPNOW_DEBUG
                Serial.println(
                    "[ESP-NOW] Kart no disponible.");
                Serial.println(
                    "[ESP-NOW] Intentando Laboratorio...");
#endif
            }
            else
            {
                //-----------------------------------------------------------------
                // Ninguno respondió.
                //
                // Volvemos a intentar desde el Kart.
                //-----------------------------------------------------------------

                m_receiverCandidate = KartReceiverIndex;

#if ESPNOW_DEBUG
                Serial.println(
                    "[ESP-NOW] Ningun receptor disponible.");
                Serial.println(
                    "[ESP-NOW] Reiniciando busqueda...");
#endif
            }
        }
    }


    //=========================================================================
    // Si ya tenemos receptor seleccionado
    //=========================================================================

    if (m_receiverSelected)
    {
        const esp_err_t result =
            esp_now_send(
                m_selectedReceiver.data(),
                packet,
                length);

        return IsSuccess(result);
    }


    //=========================================================================
    // Selección automática
    //=========================================================================

    Types::MacAddress candidateMac{};

    if (m_receiverCandidate == KartReceiverIndex)
    {
        candidateMac =
            TransmitterConfig::ReceiverMacAddressKart;

#if ESPNOW_DEBUG
        Serial.println(
            "[ESP-NOW] Probando Kart...");
#endif
    }
    else
    {
        candidateMac =
            TransmitterConfig::ReceiverMacAddressLab;

#if ESPNOW_DEBUG
        Serial.println(
            "[ESP-NOW] Probando Laboratorio...");
#endif
    }


    //-------------------------------------------------------------------------
    // Si la MAC no está configurada, la consideramos no disponible.
    //-------------------------------------------------------------------------

    if (!IsValidMac(candidateMac))
    {
#if ESPNOW_DEBUG
        if (m_receiverCandidate == KartReceiverIndex)
        {
            Serial.println(
                "[ESP-NOW] MAC del Kart invalida.");
        }
        else
        {
            Serial.println(
                "[ESP-NOW] MAC de Laboratorio no configurada.");
        }
#endif

        m_transmissionResultAvailable = true;
        m_lastTransmissionSuccessful = false;

        return false;
    }


    //=========================================================================
    // Enviar solamente al candidato actual
    //=========================================================================

    const esp_err_t result =
        esp_now_send(
            candidateMac.data(),
            packet,
            length);


    if (!IsSuccess(result))
    {
        //---------------------------------------------------------------------
        // Fallo inmediato de esp_now_send().
        //---------------------------------------------------------------------

        m_transmissionPending = false;
        m_transmissionResultAvailable = true;
        m_lastTransmissionSuccessful = false;

        return false;
    }


    //-------------------------------------------------------------------------
    // El paquete fue aceptado para transmisión.
    //
    // Ahora esperamos OnDataSent().
    //-------------------------------------------------------------------------

    m_transmissionPending = true;
    m_transmissionResultAvailable = false;

    return true;
}


//=============================================================================
// ReceiveVehicleStatus
//=============================================================================

bool ESPNowHandler::ReceiveVehicleStatus(
    Protocol::VehicleStatus& status) noexcept
{
    if (!m_vehicleStatusAvailable)
    {
        return false;
    }

    status = m_lastVehicleStatus;

    m_vehicleStatusAvailable = false;

    return true;
}


//=============================================================================
// Callback: OnDataSent
//=============================================================================

void ESPNowHandler::OnDataSent(
    const wifi_tx_info_t* txInfo,
    const esp_now_send_status_t status) noexcept
{
    (void)txInfo;

    if (s_instance == nullptr)
    {
        return;
    }


    //-------------------------------------------------------------------------
    // El callback solamente actualiza el resultado.
    //
    // La selección del receptor se hace posteriormente desde Send(),
    // evitando modificar estructuras complejas desde el callback WiFi.
    //-------------------------------------------------------------------------

    s_instance->m_lastTransmissionSuccessful =
        (status == ESP_NOW_SEND_SUCCESS);

    s_instance->m_transmissionPending = false;

    s_instance->m_transmissionResultAvailable = true;
}


//=============================================================================
// Callback: OnDataReceive
//=============================================================================

void ESPNowHandler::OnDataReceive(
    const esp_now_recv_info_t* info,
    const std::uint8_t* data,
    const int length) noexcept
{
    if (s_instance == nullptr ||
        info == nullptr ||
        data == nullptr ||
        length <= 0)
    {
        return;
    }


    //=========================================================================
    // Tamaño esperado
    //=========================================================================

    const std::size_t expectedSize =
        Protocol::PacketSize<Protocol::VehicleStatus>();

    if (static_cast<std::size_t>(length) < expectedSize)
    {
        return;
    }


    //=========================================================================
    // Deserializar
    //=========================================================================

    Protocol::Packet<Protocol::VehicleStatus> packet{};

    if (!Protocol::PacketSerializer::Deserialize(
            data,
            static_cast<std::size_t>(length),
            packet))
    {
        return;
    }


    //=========================================================================
    // Validar tipo
    //=========================================================================

    if (packet.header.type !=
        Protocol::PacketType::VehicleStatus)
    {
        return;
    }


    //=========================================================================
    // Validar payload
    //=========================================================================

    if (packet.header.payloadSize !=
        sizeof(Protocol::VehicleStatus))
    {
        return;
    }


    //=========================================================================
    // Identificar receptor
    //=========================================================================

    Types::MacAddress senderMac{};

    std::memcpy(
        senderMac.data(),
        info->src_addr,
        senderMac.size());


    //=========================================================================
    // Solamente aceptamos respuestas de nuestros receptores configurados.
    //=========================================================================

    if (!s_instance->IsConfiguredReceiver(senderMac))
    {
        return;
    }


    //=========================================================================
    // Si todavía no había receptor seleccionado, la respuesta confirma
    // directamente cuál receptor está disponible.
    //=========================================================================

    if (!s_instance->m_receiverSelected)
    {
        s_instance->SelectReceiver(senderMac);

        s_instance->m_transmissionPending = false;
        s_instance->m_transmissionResultAvailable = false;
    }
    else
    {
        //---------------------------------------------------------------------
        // Si ya tenemos receptor seleccionado, ignoramos cualquier respuesta
        // proveniente de otro receptor.
        //---------------------------------------------------------------------

        if (senderMac != s_instance->m_selectedReceiver)
        {
            return;
        }
    }


    //=========================================================================
    // Guardar VehicleStatus
    //=========================================================================

    s_instance->m_lastVehicleStatus =
        packet.payload;

    s_instance->m_vehicleStatusAvailable = true;
}


//=============================================================================
// InitializeWiFi
//=============================================================================

bool ESPNowHandler::InitializeWiFi() noexcept
{
    WiFi.mode(WIFI_STA);

    WiFi.disconnect();

    const esp_err_t result =
        esp_wifi_set_channel(
            MK::RadioConfig::Channel,
            WIFI_SECOND_CHAN_NONE);

    return IsSuccess(result);
}


//=============================================================================
// InitializeESPNow
//=============================================================================

bool ESPNowHandler::InitializeESPNow() noexcept
{
    if (!IsSuccess(esp_now_init()))
    {
        return false;
    }


    //-------------------------------------------------------------------------
    // Callback de transmisión
    //-------------------------------------------------------------------------

    if (!IsSuccess(
            esp_now_register_send_cb(
                &ESPNowHandler::OnDataSent)))
    {
        return false;
    }


    //-------------------------------------------------------------------------
    // Callback de recepción
    //-------------------------------------------------------------------------

    if (!IsSuccess(
            esp_now_register_recv_cb(
                &ESPNowHandler::OnDataReceive)))
    {
        return false;
    }


    return true;
}


//=============================================================================
// RegisterPeers
//=============================================================================

bool ESPNowHandler::RegisterPeers() noexcept
{
    //-------------------------------------------------------------------------
    // Kart
    //-------------------------------------------------------------------------

    if (!RegisterPeer(
            TransmitterConfig::ReceiverMacAddressKart))
    {
        return false;
    }


    //-------------------------------------------------------------------------
    // Laboratorio
    //
    // Solamente lo registramos si tiene una MAC válida.
    // Esto permite que el Transmitter siga funcionando con el Kart mientras
    // todavía no se configura la MAC del laboratorio.
    //-------------------------------------------------------------------------

    if (IsValidMac(
            TransmitterConfig::ReceiverMacAddressLab))
    {
        if (!RegisterPeer(
                TransmitterConfig::ReceiverMacAddressLab))
        {
            return false;
        }
    }


    return true;
}


//=============================================================================
// RegisterPeer
//=============================================================================

bool ESPNowHandler::RegisterPeer(
    const Types::MacAddress& macAddress) noexcept
{
    if (!IsValidMac(macAddress))
    {
        return false;
    }


    esp_now_peer_info_t peer =
        CreatePeerInfo(macAddress);


    //-------------------------------------------------------------------------
    // Evitar agregar un peer duplicado.
    //-------------------------------------------------------------------------

    if (esp_now_is_peer_exist(
            macAddress.data()))
    {
        return true;
    }


    const esp_err_t result =
        esp_now_add_peer(&peer);


    if (!IsSuccess(result))
    {
        return false;
    }


    //-------------------------------------------------------------------------
    // Guardamos la estructura correspondiente.
    //-------------------------------------------------------------------------

    if (macAddress ==
        TransmitterConfig::ReceiverMacAddressKart)
    {
        m_peerKart = peer;
    }
    else if (macAddress ==
             TransmitterConfig::ReceiverMacAddressLab)
    {
        m_peerLab = peer;
    }


    return true;
}


//=============================================================================
// IsValidMac
//=============================================================================

bool ESPNowHandler::IsValidMac(
    const Types::MacAddress& macAddress) const noexcept
{
    for (const auto value : macAddress)
    {
        if (value != 0x00)
        {
            return true;
        }
    }

    return false;
}


//=============================================================================
// IsConfiguredReceiver
//=============================================================================

bool ESPNowHandler::IsConfiguredReceiver(
    const Types::MacAddress& macAddress) const noexcept
{
    if (macAddress ==
        TransmitterConfig::ReceiverMacAddressKart)
    {
        return true;
    }

    if (IsValidMac(
            TransmitterConfig::ReceiverMacAddressLab) &&
        macAddress ==
        TransmitterConfig::ReceiverMacAddressLab)
    {
        return true;
    }

    return false;
}


//=============================================================================
// SelectReceiver
//=============================================================================

void ESPNowHandler::SelectReceiver(
    const Types::MacAddress& macAddress) noexcept
{
    m_selectedReceiver = macAddress;
    m_receiverSelected = true;

    m_transmissionPending = false;
    m_transmissionResultAvailable = false;


#if ESPNOW_DEBUG

    Serial.print(
        "[ESP-NOW] Receptor seleccionado: ");

    for (std::size_t i = 0;
         i < macAddress.size();
         ++i)
    {
        if (i > 0)
        {
            Serial.print(":");
        }

        if (macAddress[i] < 0x10)
        {
            Serial.print("0");
        }

        Serial.print(
            macAddress[i],
            HEX);
    }

    Serial.println();

#endif
}


//=============================================================================
// IsInitialized
//=============================================================================

bool ESPNowHandler::IsInitialized() const noexcept
{
    return m_initialized;
}

} // namespace MK