/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : PacketSerializer.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Serializador genérico para cualquier Packet<T>.
 ******************************************************************************/

#ifndef MK_RECEIVER_PACKETSERIALIZER_H
#define MK_RECEIVER_PACKETSERIALIZER_H

//=============================================================================
// Includes
//=============================================================================

#include <cstring>

#include "Packet.h"

namespace MK::Protocol
{

class PacketSerializer final
{
public:

    //=========================================================================
    // Serialización
    //=========================================================================

    template<typename TPayload>
    static bool Serialize(
        const Packet<TPayload>& packet,
        std::uint8_t* buffer,
        std::size_t length) noexcept
    {
        if (buffer == nullptr)
        {
            return false;
        }

        constexpr std::size_t packetSize =
            sizeof(Packet<TPayload>);

        if (length < packetSize)
        {
            return false;
        }

        std::memcpy(
            buffer,
            &packet,
            packetSize);

        return true;
    }

    //=========================================================================
    // Deserialización
    //=========================================================================

    template<typename TPayload>
    static bool Deserialize(
        const std::uint8_t* buffer,
        std::size_t length,
        Packet<TPayload>& packet) noexcept
    {
        if (buffer == nullptr)
        {
            return false;
        }

        constexpr std::size_t packetSize =
            sizeof(Packet<TPayload>);

        if (length < packetSize)
        {
            return false;
        }

        std::memcpy(
            &packet,
            buffer,
            packetSize);

        return true;
    }
};

} // namespace MK::Protocol

#endif // MK_RECEIVER_PACKETSERIALIZER_H