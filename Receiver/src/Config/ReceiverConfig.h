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

//=============================================================================
// Desarrollo
//=============================================================================

inline constexpr bool HardwareTestMode = true;

//=============================================================================
// Comunicación
//=============================================================================

inline constexpr Types::MacAddress TransmitterMacAddress =
{
    0xCC,
    0xDB,
    0xA7,
    0x3D,
    0xA9,
    0xF0
};

} // namespace MK::ReceiverConfig

#endif