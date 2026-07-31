/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Packet.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Define la estructura base de todos los paquetes intercambiados
 * mediante Bluetooth.
 *
 * Todo paquete enviado o recibido comienza con un PacketHeader.
 ******************************************************************************/

#ifndef MK_RECEIVER_PACKET_H
#define MK_RECEIVER_PACKET_H

//=============================================================================
// Includes
//=============================================================================

#include <cstddef>
#include <cstdint>

#include "PacketType.h"

namespace MK::Protocol
{

//=============================================================================
// PacketHeader
//=============================================================================

struct PacketHeader
{
    //---------------------------------------------------------------------
    // Tipo de paquete
    //---------------------------------------------------------------------

    PacketType type;

    //---------------------------------------------------------------------
    // Tamaño del payload
    //---------------------------------------------------------------------

    std::uint16_t payloadSize;
};

//=============================================================================
// Packet
//=============================================================================

template<typename TPayload>
struct Packet
{
    PacketHeader header;

    TPayload payload;
};

//=============================================================================
// Utilidades
//=============================================================================

template<typename TPayload>
constexpr std::size_t PacketSize() noexcept
{
    return sizeof(Packet<TPayload>);
}

} // namespace MK::Protocol

#endif // MK_RECEIVER_PACKET_H