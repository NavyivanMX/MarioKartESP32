/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : PacketType.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Identificadores de los paquetes soportados por el protocolo Bluetooth.
 ******************************************************************************/

#ifndef MK_RECEIVER_PACKETTYPE_H
#define MK_RECEIVER_PACKETTYPE_H

//=============================================================================
// Includes
//=============================================================================

#include <cstdint>

namespace MK::Protocol
{

//=============================================================================
// PacketType
//=============================================================================

enum class PacketType : std::uint8_t
{
    //---------------------------------------------------------------------
    // Android -> Receiver
    //---------------------------------------------------------------------

    DriverCommand = 0x01,

    //---------------------------------------------------------------------
    // Receiver -> Android
    //---------------------------------------------------------------------

    VehicleStatus = 0x02
};

} // namespace MK::Protocol

#endif // MK_RECEIVER_PACKETTYPE_H