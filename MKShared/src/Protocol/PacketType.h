/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : PacketType.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Identificadores de los paquetes soportados por el protocolo
 * de comunicación de MarioKart ESP32 RC.
 ******************************************************************************/

#ifndef MK_SHARED_PACKETTYPE_H
#define MK_SHARED_PACKETTYPE_H

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
    //------------------------------------------------------------------------
    // Transmitter / Android -> Receiver
    //------------------------------------------------------------------------

    DriverCommand = 0x01,

    //------------------------------------------------------------------------
    // Receiver -> Transmitter / Android
    //------------------------------------------------------------------------

    VehicleStatus = 0x02,

    //------------------------------------------------------------------------
    // Transmitter / Android -> Receiver
    //------------------------------------------------------------------------

    RequestVehicleStatus = 0x03
};

} // namespace MK::Protocol

#endif // MK_SHARED_PACKETTYPE_H
