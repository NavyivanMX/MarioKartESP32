/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DeviceConfig.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Configuración específica del hardware del dispositivo.
 ******************************************************************************/

#ifndef MK_SHARED_DEVICE_CONFIG_H
#define MK_SHARED_DEVICE_CONFIG_H

#include "Types.h"

namespace MK::DeviceConfig
{

//=============================================================================
// ESP-NOW
//=============================================================================

namespace Radio
{

/// Canal utilizado por ESP-NOW.
inline constexpr std::uint8_t Channel = 1;

/// Encriptación ESP-NOW.
inline constexpr bool Encryption = false;

/// Dirección MAC del Receiver.
inline constexpr Types::MacAddress ReceiverMacAddress =
{
    0xCC,
    0xDB,
    0xA7,
    0x3E,
    0xD7,
    0x74
};

} // namespace Radio

} // namespace MK::DeviceConfig

#endif // MK_SHARED_DEVICE_CONFIG_H