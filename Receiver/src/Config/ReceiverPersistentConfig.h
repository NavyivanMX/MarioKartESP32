/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ReceiverPersistentConfig.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Configuración persistente del Receiver.
 *
 * Guarda en NVS la cantidad de LEDs de:
 *   - AmbientLight
 *   - RearLight
 ******************************************************************************/

#ifndef MK_RECEIVER_PERSISTENT_CONFIG_H
#define MK_RECEIVER_PERSISTENT_CONFIG_H

#include <cstdint>
#include <Preferences.h>

namespace MK
{

class ReceiverPersistentConfig final
{
public:

    static constexpr std::uint8_t DefaultAmbientLedCount = 6;
    static constexpr std::uint8_t DefaultRearLedCount = 2;

    [[nodiscard]]
    bool Begin() noexcept
    {
        if (!m_preferences.begin("MarioKart", false))
        {
            m_initialized = false;
            return false;
        }

        m_initialized = true;
        return Load();
    }

    [[nodiscard]]
    std::uint8_t GetAmbientLedCount() const noexcept
    {
        return m_ambientLedCount;
    }

    [[nodiscard]]
    std::uint8_t GetRearLedCount() const noexcept
    {
        return m_rearLedCount;
    }

    [[nodiscard]]
    bool SetAmbientLedCount(std::uint8_t count) noexcept
    {
        if (!m_initialized || !IsValidCount(count))
            return false;

        if (m_preferences.putUChar("ambientLeds", count) != 1)
            return false;

        m_ambientLedCount = count;
        return true;
    }

    [[nodiscard]]
    bool SetRearLedCount(std::uint8_t count) noexcept
    {
        if (!m_initialized || !IsValidCount(count))
            return false;

        if (m_preferences.putUChar("rearLeds", count) != 1)
            return false;

        m_rearLedCount = count;
        return true;
    }

    [[nodiscard]]
    bool Clear() noexcept
    {
        if (!m_initialized)
            return false;

        const bool ambientRemoved =
            m_preferences.remove("ambientLeds");

        const bool rearRemoved =
            m_preferences.remove("rearLeds");

        // "false" también significa que la clave simplemente no existía.
        (void)ambientRemoved;
        (void)rearRemoved;

        m_ambientLedCount = DefaultAmbientLedCount;
        m_rearLedCount = DefaultRearLedCount;

        return true;
    }

    [[nodiscard]]
    bool IsInitialized() const noexcept
    {
        return m_initialized;
    }

    ~ReceiverPersistentConfig()
    {
        m_preferences.end();
    }

private:

    [[nodiscard]]
    bool Load() noexcept
    {
        if (!m_initialized)
            return false;

        m_ambientLedCount =
            m_preferences.getUChar(
                "ambientLeds",
                DefaultAmbientLedCount);

        m_rearLedCount =
            m_preferences.getUChar(
                "rearLeds",
                DefaultRearLedCount);

        if (!IsValidCount(m_ambientLedCount))
            m_ambientLedCount = DefaultAmbientLedCount;

        if (!IsValidCount(m_rearLedCount))
            m_rearLedCount = DefaultRearLedCount;

        Serial.printf(
            "NVS: Ambient LEDs: %u\n",
            static_cast<unsigned int>(m_ambientLedCount));

        Serial.printf(
            "NVS: Rear LEDs: %u\n",
            static_cast<unsigned int>(m_rearLedCount));

        return true;
    }

    [[nodiscard]]
    static bool IsValidCount(std::uint8_t count) noexcept
    {
        // Evita valores 0 y tamaños absurdamente grandes.
        return count >= 1 && count <= 255;
    }

private:

    Preferences m_preferences;
    std::uint8_t m_ambientLedCount{DefaultAmbientLedCount};
    std::uint8_t m_rearLedCount{DefaultRearLedCount};
    bool m_initialized{false};
};

}

#endif
