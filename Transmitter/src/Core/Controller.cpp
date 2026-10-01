/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Controller.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del controlador principal del Transmitter.
 *
 * Configuración Serial:
 *
 *   MKCFG GET
 *
 *       Consulta el MAC personalizado almacenado en NVS.
 *
 *
 *   MKCFG SET AA:BB:CC:DD:EE:FF
 *
 *       Guarda un MAC personalizado en NVS.
 *
 *
 *   MKCFG CLEAR
 *
 *       Elimina el MAC personalizado de NVS.
 *
 ******************************************************************************/

#include "Controller.h"

#include <cstring>
#include <cstdlib>

#include <Arduino.h>

#include <MKShared.h>

namespace MK
{

//=============================================================================
// Begin
//=============================================================================

bool Controller::Begin()
{
    m_consoleLogger.Begin();

    m_consoleLogger.LogBoot();


    m_inputManager.Begin();


    if (!m_statusLightController.Begin())
    {
        m_consoleLogger.LogError(
            "Status light initialization failed.");

        return false;
    }


    //=========================================================================
    // Configuración persistente
    //=========================================================================

    if (!m_persistentConfig.Begin())
    {
        m_consoleLogger.LogError(
            "Persistent configuration initialization failed.");

        return false;
    }


    Serial.println();
    Serial.println(
        "========== Transmitter Configuration ==========");


    if (m_persistentConfig.HasCustomReceiverMac())
    {
        Serial.print(
            "Custom Receiver MAC: ");

        PrintMacAddress(
            m_persistentConfig.GetCustomReceiverMac());

        Serial.println();
    }
    else
    {
        Serial.println(
            "Custom Receiver MAC: NONE");
    }


    Serial.println(
        "===============================================");


    //=========================================================================
    // ESP-NOW
    //=========================================================================

    if constexpr (!TransmitterConfig::InputTestMode)
    {
        if (!m_espNowHandler.Begin(
                m_persistentConfig.HasCustomReceiverMac(),
                m_persistentConfig.GetCustomReceiverMac()))
        {
            m_consoleLogger.LogError(
                "ESP-NOW initialization failed.");

            return false;
        }
    }


    pinMode(
        Pins::Haptic,
        OUTPUT);

    digitalWrite(
        Pins::Haptic,
        LOW);


    m_lastTransmitTime =
        millis();


    m_vehicleStatus = {};

    m_hasVehicleStatus = true;


    return true;
}


//=============================================================================
// Update
//=============================================================================

void Controller::Update() noexcept
{
    //=========================================================================
    // Procesar comandos de configuración por Serial
    //=========================================================================

    ProcessSerialConfiguration();


    //=========================================================================
    // Input
    //=========================================================================

    m_inputManager.Update();


    const auto& command =
        m_inputManager.GetDriverCommand();


    //=========================================================================
    // VehicleStatus
    //=========================================================================

    if constexpr (!TransmitterConfig::InputTestMode)
    {
        UpdateVehicleStatus();
    }


    //=========================================================================
    // Status Light
    //=========================================================================

    m_statusLightController.Update(
        command,
        m_hasVehicleStatus
            ? m_vehicleStatus.drivingProfile
            : VehicleProfiles::DrivingProfileId::Rookie);


    //=========================================================================
    // Periodo de transmisión
    //=========================================================================

    const std::uint32_t now =
        millis();


    if ((now - m_lastTransmitTime) <
        TransmitterConfig::TransmitPeriodMs)
    {
        return;
    }


    m_lastTransmitTime =
        now;


    //=========================================================================
    // Turbo / Haptic
    //=========================================================================

    using Types::Vehicle::Turbo;


    static bool lastTurbo = false;


    const bool turbo =
        command.turbo == Turbo::Enabled;


    if (turbo != lastTurbo)
    {
        Serial.printf(
            "Turbo -> %s\n",
            turbo ? "ON" : "OFF");


        lastTurbo = turbo;
    }


    digitalWrite(
        Pins::Haptic,
        turbo ? HIGH : LOW);


    //=========================================================================
    // Log de comando
    //=========================================================================

    if (command != m_lastLoggedCommand)
    {
        m_consoleLogger.Log(
            command);

        m_lastLoggedCommand =
            command;
    }


    //=========================================================================
    // Crear paquete
    //=========================================================================

    Protocol::Packet<
        Protocol::DriverCommand> packet;


    packet.header.type =
        Protocol::PacketType::DriverCommand;


    packet.header.payloadSize =
        sizeof(Protocol::DriverCommand);


    packet.payload =
        command;


    //=========================================================================
    // Serializar
    //=========================================================================

    std::uint8_t buffer[
        Protocol::PacketSize<
            Protocol::DriverCommand>()];


    if (!Protocol::PacketSerializer::Serialize(
            packet,
            buffer,
            sizeof(buffer)))
    {
        return;
    }


#if MK_DEBUG_PACKET_SERIALIZER

    Serial.println();

    Serial.println(
        "========== PacketSerializer ==========");


    for (std::size_t i = 0;
         i < sizeof(buffer);
         ++i)
    {
        if (buffer[i] < 16)
            Serial.print('0');

        Serial.print(
            buffer[i],
            HEX);

        Serial.print(' ');
    }


    Serial.println();


    Serial.println(
        "======================================");

#endif


    //=========================================================================
    // Transmitir
    //=========================================================================

    m_espNowHandler.Send(
        buffer,
        sizeof(buffer));
}


//=============================================================================
// UpdateVehicleStatus
//=============================================================================

void Controller::UpdateVehicleStatus() noexcept
{
    Protocol::VehicleStatus status{};


    if (!m_espNowHandler.ReceiveVehicleStatus(status))
        return;


    m_vehicleStatus =
        status;


    m_hasVehicleStatus =
        true;


    Serial.print(
        "VehicleStatus -> Profile: ");


    Serial.println(
        static_cast<std::uint8_t>(
            m_vehicleStatus.drivingProfile));
}


//=============================================================================
// ProcessSerialConfiguration
//=============================================================================

void Controller::ProcessSerialConfiguration() noexcept
{
    while (Serial.available() > 0)
    {
        const char character =
            static_cast<char>(
                Serial.read());


        //=====================================================================
        // Fin de línea
        //=====================================================================

        if (character == '\n' ||
            character == '\r')
        {
            if (m_serialBufferLength == 0)
                continue;


            m_serialBuffer[
                m_serialBufferLength] =
                '\0';


            //=================================================================
            // MKCFG GET
            //=================================================================

            if (std::strcmp(
                    m_serialBuffer,
                    "MKCFG GET") == 0)
            {
                Serial.print(
                    "MKCFG ");


                if (m_persistentConfig.HasCustomReceiverMac())
                {
                    Serial.print(
                        "CUSTOM_MAC=");


                    PrintMacAddress(
                        m_persistentConfig.GetCustomReceiverMac());


                    Serial.println();
                }
                else
                {
                    Serial.println(
                        "CUSTOM_MAC=NONE");
                }
            }


            //=================================================================
            // MKCFG CLEAR
            //=================================================================

            else if (std::strcmp(
                         m_serialBuffer,
                         "MKCFG CLEAR") == 0)
            {
                if (m_persistentConfig.ClearCustomReceiverMac())
                {
                    Serial.println(
                        "MKCFG OK");
                }
                else
                {
                    Serial.println(
                        "MKCFG ERROR");
                }
            }


            //=================================================================
            // MKCFG SET
            //
            // Formato:
            //
            // MKCFG SET AA:BB:CC:DD:EE:FF
            //=================================================================

            else if (
                std::strncmp(
                    m_serialBuffer,
                    "MKCFG SET ",
                    10) == 0)
            {
                const char* macText =
                    m_serialBuffer + 10;


                Types::MacAddress macAddress{};


                if (!ParseMacAddress(
                        macText,
                        macAddress))
                {
                    Serial.println(
                        "MKCFG ERROR INVALID_MAC");
                }
                else if (
                    !m_persistentConfig.SetCustomReceiverMac(
                        macAddress))
                {
                    Serial.println(
                        "MKCFG ERROR SAVE");
                }
                else
                {
                    Serial.println(
                        "MKCFG OK");

                    Serial.print(
                        "MKCFG CUSTOM_MAC=");

                    PrintMacAddress(
                        macAddress);

                    Serial.println();

                    Serial.println(
                        "MKCFG RESTART_REQUIRED");
                }
            }


            //=================================================================
            // Comando desconocido
            //=================================================================

            else
            {
                Serial.println(
                    "MKCFG ERROR UNKNOWN_COMMAND");
            }


            //=================================================================
            // Reiniciar buffer
            //=================================================================

            m_serialBufferLength = 0;
        }


        //=====================================================================
        // Caracter normal
        //=====================================================================

        else
        {
            if (m_serialBufferLength <
                sizeof(m_serialBuffer) - 1)
            {
                m_serialBuffer[
                    m_serialBufferLength++] =
                    character;
            }
            else
            {
                // Buffer overflow.
                m_serialBufferLength = 0;

                Serial.println(
                    "MKCFG ERROR BUFFER_OVERFLOW");
            }
        }
    }
}


//=============================================================================
// ParseMacAddress
//=============================================================================

bool Controller::ParseMacAddress(
    const char* text,
    Types::MacAddress& macAddress) noexcept
{
    if (text == nullptr)
        return false;


    unsigned int values[6]{};


    const int parsed =
        std::sscanf(
            text,
            "%2x:%2x:%2x:%2x:%2x:%2x",
            &values[0],
            &values[1],
            &values[2],
            &values[3],
            &values[4],
            &values[5]);


    if (parsed != 6)
        return false;


    for (std::size_t i = 0;
         i < macAddress.size();
         ++i)
    {
        if (values[i] > 0xFF)
            return false;


        macAddress[i] =
            static_cast<std::uint8_t>(
                values[i]);
    }


    // No aceptar 00:00:00:00:00:00

    bool hasNonZeroByte = false;


    for (const auto value : macAddress)
    {
        if (value != 0x00)
        {
            hasNonZeroByte = true;
            break;
        }
    }


    return hasNonZeroByte;
}


//=============================================================================
// PrintMacAddress
//=============================================================================

void Controller::PrintMacAddress(
    const Types::MacAddress& macAddress) noexcept
{
    for (std::size_t i = 0;
         i < macAddress.size();
         ++i)
    {
        if (i != 0)
            Serial.print(':');


        if (macAddress[i] < 0x10)
            Serial.print('0');


        Serial.print(
            macAddress[i],
            HEX);
    }
}

}