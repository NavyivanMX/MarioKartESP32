/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : TransmitterPersistentConfig.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Configuración persistente del Transmitter.
 *
 * Guarda en NVS un MAC de Receiver personalizado.
 *
 * El MAC se almacena como texto:
 *
 *     AA:BB:CC:DD:EE:FF
 *
 ******************************************************************************/

#ifndef MK_TRANSMITTER_PERSISTENT_CONFIG_H
#define MK_TRANSMITTER_PERSISTENT_CONFIG_H

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <Preferences.h>
#include <MKShared.h>

namespace MK
{

class TransmitterPersistentConfig final
{
public:

    TransmitterPersistentConfig() = default;

    ~TransmitterPersistentConfig()
    {
        m_preferences.end();
    }


    //=========================================================================
    // Inicialización
    //=========================================================================

    [[nodiscard]]
    bool Begin() noexcept
    {
        if (!m_preferences.begin(
                "MarioKart",
                false))
        {
            m_initialized = false;

            return false;
        }


        m_initialized = true;


        return Load();
    }


    //=========================================================================
    // MAC personalizado
    //=========================================================================

    [[nodiscard]]
    bool HasCustomReceiverMac() const noexcept
    {
        return m_hasCustomReceiverMac;
    }


    [[nodiscard]]
    const Types::MacAddress&
    GetCustomReceiverMac() const noexcept
    {
        return m_customReceiverMac;
    }


    //=========================================================================
    // Guardar MAC personalizado
    //=========================================================================

    [[nodiscard]]
    bool SetCustomReceiverMac(
        const Types::MacAddress& macAddress) noexcept
    {
        if (!m_initialized)
        {
            Serial.println(
                "NVS: not initialized.");

            return false;
        }


        if (!IsValidMac(macAddress))
        {
            Serial.println(
                "NVS: invalid MAC.");

            return false;
        }


        //=====================================================================
        // Convertir MAC a texto
        //=====================================================================

        char macText[18] = {};


        const int result =
            std::snprintf(
                macText,
                sizeof(macText),
                "%02X:%02X:%02X:%02X:%02X:%02X",
                macAddress[0],
                macAddress[1],
                macAddress[2],
                macAddress[3],
                macAddress[4],
                macAddress[5]);


        if (result != 17)
        {
            Serial.println(
                "NVS: MAC conversion failed.");

            return false;
        }


        //=====================================================================
        // Guardar como String
        //=====================================================================

        const std::size_t written =
            m_preferences.putString(
                "customMac",
                macText);


        if (written != 17)
        {
            Serial.printf(
                "NVS: putString failed. Written: %u\n",
                static_cast<unsigned int>(written));

            return false;
        }


        //=====================================================================
        // Actualizar RAM
        //=====================================================================

        m_customReceiverMac =
            macAddress;

        m_hasCustomReceiverMac =
            true;


        Serial.print(
            "NVS: Custom Receiver MAC saved: ");

        Serial.println(
            macText);


        return true;
    }


    //=========================================================================
    // Eliminar MAC personalizado
    //=========================================================================

    [[nodiscard]]
    bool ClearCustomReceiverMac() noexcept
    {
        if (!m_initialized)
            return false;


        const bool removed =
            m_preferences.remove(
                "customMac");


        m_customReceiverMac = {};

        m_hasCustomReceiverMac = false;


        if (!removed)
        {
            // remove() puede devolver false si la clave no existía.
            //
            // Eso no representa un error para nosotros porque el resultado
            // final que buscamos es que no exista el MAC personalizado.
            return true;
        }


        return true;
    }


private:

    //=========================================================================
    // Cargar configuración desde NVS
    //=========================================================================

    [[nodiscard]]
    bool Load() noexcept
    {
        if (!m_initialized)
            return false;


        char macText[32] = {};


        const std::size_t length =
            m_preferences.getString(
                "customMac",
                macText,
                sizeof(macText));


        //=====================================================================
        // No existe configuración
        //=====================================================================

        if (length == 0)
        {
            m_customReceiverMac = {};

            m_hasCustomReceiverMac = false;

            return true;
        }


        //=====================================================================
        // El formato debe ser:
        //
        // AA:BB:CC:DD:EE:FF
        //=====================================================================

        unsigned int values[6] = {};


        const int parsed =
            std::sscanf(
                macText,
                "%2x:%2x:%2x:%2x:%2x:%2x",
                &values[0],
                &values[1],
                &values[2],
                &values[3],
                &values[4],
                &values[5]);


        if (parsed != 6)
        {
            Serial.print(
                "NVS: Invalid stored MAC: ");

            Serial.println(
                macText);


            m_customReceiverMac = {};

            m_hasCustomReceiverMac = false;


            return false;
        }


        //=====================================================================
        // Convertir a MacAddress
        //=====================================================================

        for (std::size_t i = 0;
             i < m_customReceiverMac.size();
             ++i)
        {
            if (values[i] > 0xFF)
            {
                m_customReceiverMac = {};

                m_hasCustomReceiverMac = false;

                return false;
            }


            m_customReceiverMac[i] =
                static_cast<std::uint8_t>(
                    values[i]);
        }


        //=====================================================================
        // Validar
        //=====================================================================

        if (!IsValidMac(
                m_customReceiverMac))
        {
            m_customReceiverMac = {};

            m_hasCustomReceiverMac = false;

            return false;
        }


        m_hasCustomReceiverMac = true;


        Serial.print(
            "NVS: Custom Receiver MAC loaded: ");

        Serial.println(
            macText);


        return true;
    }


    //=========================================================================
    // Validar MAC
    //=========================================================================

    [[nodiscard]]
    static bool IsValidMac(
        const Types::MacAddress& macAddress) noexcept
    {
        for (const auto value : macAddress)
        {
            if (value != 0x00)
                return true;
        }


        return false;
    }


private:

    Preferences m_preferences;

    Types::MacAddress m_customReceiverMac{};

    bool m_hasCustomReceiverMac = false;

    bool m_initialized = false;
};

}

#endif