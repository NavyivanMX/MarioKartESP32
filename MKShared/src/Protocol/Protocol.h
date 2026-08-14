/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Protocol.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Punto de entrada del protocolo de comunicación compartido entre
 * Transmitter, Receiver y Android.
 *
 * Reúne todos los tipos, estructuras y paquetes utilizados para la
 * comunicación entre dispositivos.
 ******************************************************************************/

#ifndef MK_SHARED_PROTOCOL_H
#define MK_SHARED_PROTOCOL_H

//=============================================================================
// Includes
//=============================================================================

#include "Types/Vehicle/Direction.h"
#include "Types/Vehicle/DriveMode.h"
#include "Types/Vehicle/Steering.h"
#include "Types/Vehicle/Turbo.h"

#include "DriverCommand.h"
#include "VehicleStatus.h"

#include "Packet.h"
#include "PacketType.h"
#include "PacketSerializer.h"

#endif // MK_SHARED_PROTOCOL_H