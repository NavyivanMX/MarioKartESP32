/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ReceiverConfig.h
 *
 * Descripción:
 * Configuración específica del vehículo Receiver.
 ******************************************************************************/

#ifndef MK_RECEIVER_CONFIG_H
#define MK_RECEIVER_CONFIG_H

#include <MKShared.h>

namespace MK::ReceiverConfig
{

inline constexpr Types::MacAddress MacAddress =
{
    0xCC,
    0xDB,
    0xA7,
    0x3E,
    0xD7,
    0x74
};

} // namespace MK::ReceiverConfig

#endif