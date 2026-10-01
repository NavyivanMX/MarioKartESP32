/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowHandler.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación de la comunicación ESP-NOW del Transmitter.
 *
 * Selección:
 *
 *   KnownReceiverMacs[0]
 *          ↓ falla
 *   KnownReceiverMacs[1]
 *          ↓ falla
 *   KnownReceiverMacs[2]
 *          ↓ ...
 *   MAC personalizado NVS
 *
 * Cuando un Receiver responde correctamente, queda seleccionado durante
 * toda la sesión.
 ******************************************************************************/

#include "ESPNowHandler.h"

#include <cstring>

#include "src/Config/TransmitterConfig.h"


namespace MK
{

ESPNowHandler* ESPNowHandler::s_instance = nullptr;


//=============================================================================
// Begin
//=============================================================================

bool ESPNowHandler::Begin(
    bool hasCustomReceiverMac,
    const Types::MacAddress& customReceiverMac)
{
    m_customReceiverMac = customReceiverMac;
    m_hasCustomReceiverMac =
        hasCustomReceiverMac && IsValidMac(customReceiverMac);

    m_receiverCandidate = 0;

    m_selectedReceiver = {};
    m_receiverSelected = false;

    m_transmissionPending = false;
    m_transmissionResultAvailable = false;
    m_lastTransmissionSuccessful = false;

    m_vehicleStatusAvailable = false;

    if (!InitializeWiFi())
    {
        Serial.println(
            "ESP-NOW: WiFi initialization failed.");

        return false;
    }


    if (!InitializeESPNow())
    {
        Serial.println(
            "ESP-NOW: initialization failed.");

        return false;
    }


    if (!RegisterPeers())
    {
        Serial.println(
            "ESP-NOW: peer registration failed.");

        return false;
    }


    m_initialized = true;

    s_instance = this;


    Serial.println();
    Serial.println(
        "========== ESP-NOW ==========");

    Serial.printf(
        "Known receivers: %u\n",
        static_cast<unsigned int>(
            TransmitterConfig::KnownReceiverMacCount));


    if (m_hasCustomReceiverMac)
    {
        Serial.println(
            "Custom receiver: CONFIGURED");
    }
    else
    {
        Serial.println(
            "Custom receiver: NONE");
    }


    Serial.println(
        "Receiver selection: AUTO");

    Serial.println(
        "==============================");

    return true;
}


//=============================================================================
// End
//=============================================================================

void ESPNowHandler::End() noexcept
{
    if (!m_initialized)
        return;

    if (s_instance == this)
        s_instance = nullptr;

    esp_now_deinit();

    WiFi.disconnect(true);

    m_initialized = false;
    m_receiverSelected = false;

    m_selectedReceiver = {};
}


//=============================================================================
// Initialize WiFi
//=============================================================================

bool ESPNowHandler::InitializeWiFi() noexcept
{
    WiFi.mode(WIFI_STA);

    WiFi.disconnect();

    delay(100);

    Serial.print(
        "ESP-NOW: Transmitter MAC: ");

    Serial.println(
        WiFi.macAddress());

    Serial.printf(
        "ESP-NOW: Channel: %u\n",
        static_cast<unsigned int>(
            MK::RadioConfig::Channel));

    return true;
}

//=============================================================================
// Initialize ESP-NOW
//=============================================================================

bool ESPNowHandler::InitializeESPNow() noexcept
{
    if (esp_now_init() != ESP_OK)
    {
        return false;
    }


    if (esp_now_register_send_cb(
            &ESPNowHandler::OnDataSent) != ESP_OK)
    {
        return false;
    }


    if (esp_now_register_recv_cb(
            &ESPNowHandler::OnDataReceive) != ESP_OK)
    {
        return false;
    }


    return true;
}


//=============================================================================
// Register all possible peers
//=============================================================================

bool ESPNowHandler::RegisterPeers() noexcept
{
    bool registeredAtLeastOne = false;


    //-------------------------------------------------------------------------
    // Receptores conocidos
    //-------------------------------------------------------------------------

    for (std::size_t i = 0;
         i < TransmitterConfig::KnownReceiverMacCount;
         ++i)
    {
        const auto& mac =
            TransmitterConfig::KnownReceiverMacs[i];


        if (!IsValidMac(mac))
            continue;


        if (RegisterPeer(mac))
        {
            registeredAtLeastOne = true;

            Serial.printf(
                "ESP-NOW: Known receiver %u registered: ",
                static_cast<unsigned int>(i));

            for (std::size_t j = 0;
                 j < mac.size();
                 ++j)
            {
                if (j != 0)
                    Serial.print(':');

                if (mac[j] < 0x10)
                    Serial.print('0');

                Serial.print(
                    mac[j],
                    HEX);
            }

            Serial.println();
        }
    }


    //-------------------------------------------------------------------------
    // Receiver personalizado
    //-------------------------------------------------------------------------

    if (m_hasCustomReceiverMac)
    {
        bool alreadyRegistered = false;


        for (std::size_t i = 0;
             i < TransmitterConfig::KnownReceiverMacCount;
             ++i)
        {
            if (TransmitterConfig::KnownReceiverMacs[i] ==
                m_customReceiverMac)
            {
                alreadyRegistered = true;
                break;
            }
        }


        if (!alreadyRegistered)
        {
            if (RegisterPeer(m_customReceiverMac))
            {
                registeredAtLeastOne = true;

                Serial.print(
                    "ESP-NOW: Custom receiver registered: ");

                for (std::size_t j = 0;
                     j < m_customReceiverMac.size();
                     ++j)
                {
                    if (j != 0)
                        Serial.print(':');

                    if (m_customReceiverMac[j] < 0x10)
                        Serial.print('0');

                    Serial.print(
                        m_customReceiverMac[j],
                        HEX);
                }

                Serial.println();
            }
        }
    }


    return registeredAtLeastOne;
}


//=============================================================================
// Register one peer
//=============================================================================

bool ESPNowHandler::RegisterPeer(
    const Types::MacAddress& macAddress) noexcept
{
    if (!IsValidMac(macAddress))
        return false;


    if (esp_now_is_peer_exist(
            macAddress.data()))
    {
        return true;
    }


    esp_now_peer_info_t peerInfo{};


    std::memcpy(
        peerInfo.peer_addr,
        macAddress.data(),
        macAddress.size());


    peerInfo.channel =
        MK::RadioConfig::Channel;

    peerInfo.ifidx =
        WIFI_IF_STA;

    peerInfo.encrypt =
        MK::RadioConfig::Encryption;


    const esp_err_t result =
        esp_now_add_peer(&peerInfo);


    if (result != ESP_OK &&
        result != ESP_ERR_ESPNOW_EXIST)
    {
        Serial.printf(
            "ESP-NOW: esp_now_add_peer failed: %d\n",
            static_cast<int>(result));

        return false;
    }


    return true;
}


//=============================================================================
// Send
//=============================================================================

bool ESPNowHandler::Send(
    const std::uint8_t* packet,
    std::size_t length) noexcept
{
    if (!IsInitialized())
        return false;


    if (packet == nullptr ||
        length == 0)
    {
        return false;
    }


    //-------------------------------------------------------------------------
    // Esperar resultado de la transmisión anterior
    //-------------------------------------------------------------------------

    if (m_transmissionPending)
    {
        return false;
    }


    //-------------------------------------------------------------------------
    // Si todavía no tenemos Receiver seleccionado, analizar resultado
    // anterior.
    //-------------------------------------------------------------------------

    if (!m_receiverSelected &&
        m_transmissionResultAvailable)
    {
        m_transmissionResultAvailable = false;


        if (m_lastTransmissionSuccessful)
        {
            Types::MacAddress candidate{};


            if (GetCandidateMac(
                    m_receiverCandidate,
                    candidate))
            {
                SelectReceiver(candidate);

                Serial.print(
                    "ESP-NOW: Receiver selected: ");

                for (std::size_t i = 0;
                     i < candidate.size();
                     ++i)
                {
                    if (i != 0)
                        Serial.print(':');

                    if (candidate[i] < 0x10)
                        Serial.print('0');

                    Serial.print(
                        candidate[i],
                        HEX);
                }

                Serial.println();
            }
        }
        else
        {
            // ---------------------------------------------------------------
            // El candidato actual falló.
            // Avanzamos al siguiente.
            // ---------------------------------------------------------------

            ++m_receiverCandidate;


            Types::MacAddress nextCandidate{};


            if (!GetCandidateMac(
                    m_receiverCandidate,
                    nextCandidate))
            {
                Serial.println(
                    "ESP-NOW: no receiver candidates available.");

                return false;
            }


            Serial.print(
                "ESP-NOW: trying next receiver: ");

            for (std::size_t i = 0;
                 i < nextCandidate.size();
                 ++i)
            {
                if (i != 0)
                    Serial.print(':');

                if (nextCandidate[i] < 0x10)
                    Serial.print('0');

                Serial.print(
                    nextCandidate[i],
                    HEX);
            }

            Serial.println();
        }
    }


    //-------------------------------------------------------------------------
    // Receiver ya seleccionado
    //-------------------------------------------------------------------------

    if (m_receiverSelected)
    {
        const esp_err_t result =
            esp_now_send(
                m_selectedReceiver.data(),
                packet,
                length);


        if (result != ESP_OK)
        {
            return false;
        }


        m_transmissionPending = true;

        return true;
    }


    //-------------------------------------------------------------------------
    // Todavía estamos buscando Receiver
    //-------------------------------------------------------------------------

    Types::MacAddress candidate{};


    if (!GetCandidateMac(
            m_receiverCandidate,
            candidate))
    {
        return false;
    }


    const esp_err_t result =
        esp_now_send(
            candidate.data(),
            packet,
            length);


    if (result != ESP_OK)
    {
        m_transmissionResultAvailable = true;
        m_lastTransmissionSuccessful = false;

        return false;
    }


    m_transmissionPending = true;

    return true;
}


//=============================================================================
// Receive VehicleStatus
//=============================================================================

bool ESPNowHandler::ReceiveVehicleStatus(
    Protocol::VehicleStatus& status) noexcept
{
    if (!m_vehicleStatusAvailable)
        return false;


    status =
        m_lastVehicleStatus;


    m_vehicleStatusAvailable = false;

    return true;
}


//=============================================================================
// Send callback
//=============================================================================

void ESPNowHandler::OnDataSent(
    const wifi_tx_info_t*,
    esp_now_send_status_t status) noexcept
{
    if (s_instance == nullptr)
        return;


    s_instance->m_transmissionPending = false;

    s_instance->m_lastTransmissionSuccessful =
        (status == ESP_NOW_SEND_SUCCESS);

    s_instance->m_transmissionResultAvailable = true;
}


//=============================================================================
// Receive callback
//=============================================================================

void ESPNowHandler::OnDataReceive(
    const esp_now_recv_info_t* info,
    const std::uint8_t* data,
    int length) noexcept
{
    if (s_instance == nullptr ||
        info == nullptr ||
        data == nullptr ||
        length <= 0)
    {
        return;
    }


    //-------------------------------------------------------------------------
    // Convert sender MAC
    //-------------------------------------------------------------------------

    Types::MacAddress senderMac{};


    std::memcpy(
        senderMac.data(),
        info->src_addr,
        senderMac.size());


    //-------------------------------------------------------------------------
    // Solamente aceptamos mensajes de Receivers configurados
    //-------------------------------------------------------------------------

    if (!s_instance->IsConfiguredReceiver(
            senderMac))
    {
        return;
    }


    //-------------------------------------------------------------------------
    // Si todavía no tenemos Receiver seleccionado, el que acaba de responder
    // se convierte inmediatamente en el seleccionado.
    //-------------------------------------------------------------------------

    if (!s_instance->m_receiverSelected)
    {
        s_instance->SelectReceiver(senderMac);

        Serial.print(
            "ESP-NOW: Receiver selected from response: ");

        for (std::size_t i = 0;
             i < senderMac.size();
             ++i)
        {
            if (i != 0)
                Serial.print(':');

            if (senderMac[i] < 0x10)
                Serial.print('0');

            Serial.print(
                senderMac[i],
                HEX);
        }

        Serial.println();
    }
    else
    {
        // Si ya tenemos Receiver seleccionado, ignoramos cualquier otro.
        if (senderMac !=
            s_instance->m_selectedReceiver)
        {
            return;
        }
    }


    //-------------------------------------------------------------------------
    // Validar tamaño mínimo del paquete
    //-------------------------------------------------------------------------

    if (static_cast<std::size_t>(length) <
        sizeof(Protocol::PacketHeader))
    {
        return;
    }


    //-------------------------------------------------------------------------
    // Copiar header
    //-------------------------------------------------------------------------

    Protocol::PacketHeader header{};

    std::memcpy(
        &header,
        data,
        sizeof(header));


    if (header.type !=
        Protocol::PacketType::VehicleStatus)
    {
        return;
    }


    if (header.payloadSize !=
        sizeof(Protocol::VehicleStatus))
    {
        return;
    }


    const std::size_t expectedSize =
        sizeof(Protocol::PacketHeader) +
        sizeof(Protocol::VehicleStatus);


    if (static_cast<std::size_t>(length) <
        expectedSize)
    {
        return;
    }


    //-------------------------------------------------------------------------
    // Copiar VehicleStatus
    //-------------------------------------------------------------------------

    Protocol::VehicleStatus vehicleStatus{};


    std::memcpy(
        &vehicleStatus,
        data + sizeof(Protocol::PacketHeader),
        sizeof(Protocol::VehicleStatus));


    s_instance->m_lastVehicleStatus =
        vehicleStatus;

    s_instance->m_vehicleStatusAvailable = true;
}


//=============================================================================
// Get candidate MAC
//=============================================================================

bool ESPNowHandler::GetCandidateMac(
    std::size_t index,
    Types::MacAddress& macAddress) const noexcept
{
    //-------------------------------------------------------------------------
    // Primero todos los MAC conocidos
    //-------------------------------------------------------------------------

    if (index <
        TransmitterConfig::KnownReceiverMacCount)
    {
        macAddress =
            TransmitterConfig::KnownReceiverMacs[index];

        return IsValidMac(macAddress);
    }


    //-------------------------------------------------------------------------
    // Después el MAC personalizado
    //-------------------------------------------------------------------------

    if (index ==
        TransmitterConfig::KnownReceiverMacCount)
    {
        if (!m_hasCustomReceiverMac)
            return false;

        macAddress =
            m_customReceiverMac;

        return IsValidMac(macAddress);
    }


    return false;
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
            return true;
    }

    return false;
}


//=============================================================================
// IsConfiguredReceiver
//=============================================================================

bool ESPNowHandler::IsConfiguredReceiver(
    const Types::MacAddress& macAddress) const noexcept
{
    //-------------------------------------------------------------------------
    // Revisar lista conocida
    //-------------------------------------------------------------------------

    for (std::size_t i = 0;
         i < TransmitterConfig::KnownReceiverMacCount;
         ++i)
    {
        if (macAddress ==
            TransmitterConfig::KnownReceiverMacs[i])
        {
            return true;
        }
    }


    //-------------------------------------------------------------------------
    // Revisar MAC personalizado
    //-------------------------------------------------------------------------

    if (m_hasCustomReceiverMac &&
        macAddress ==
            m_customReceiverMac)
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
    if (!IsValidMac(macAddress))
        return;


    m_selectedReceiver =
        macAddress;

    m_receiverSelected = true;
}


//=============================================================================
// IsInitialized
//=============================================================================

bool ESPNowHandler::IsInitialized() const noexcept
{
    return m_initialized;
}

}