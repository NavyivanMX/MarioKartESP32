/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : PacketSerializer.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Serializa y deserializa cualquier Packet<TPayload>.
 *
 * Esta clase únicamente conoce la estructura Packet y copia el Header y
 * el Payload hacia/desde un buffer de memoria.
 ******************************************************************************/

#ifndef MK_SHARED_PACKET_SERIALIZER_H
#define MK_SHARED_PACKET_SERIALIZER_H

//=============================================================================
// Includes
//=============================================================================

#include <cstddef>
#include <cstdint>
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
    [[nodiscard]]
    static bool Serialize(
        const Packet<TPayload>& packet,
        std::uint8_t* buffer,
        std::size_t length) noexcept
    {
        if (buffer == nullptr)
        {
            return false;
        }

        if (length < PacketSize<TPayload>())
        {
            return false;
        }

        std::memcpy(
            buffer,
            &packet,
            PacketSize<TPayload>());

        return true;
    }

    //=========================================================================
    // Deserialización
    //=========================================================================

    template<typename TPayload>
    [[nodiscard]]
    static bool Deserialize(
        const std::uint8_t* buffer,
        std::size_t length,
        Packet<TPayload>& packet) noexcept
    {
        if (buffer == nullptr)
        {
            return false;
        }

        if (length < PacketSize<TPayload>())
        {
            return false;
        }

        std::memcpy(
            &packet,
            buffer,
            PacketSize<TPayload>());

        return true;
    }
};

}

#endif // MK_SHARED_PACKET_SERIALIZER_H