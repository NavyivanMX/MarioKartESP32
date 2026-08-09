/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : TransmitterConfig.h
 *
 * Descripción:
 * Configuración específica del vehículo Transmitter.
 ******************************************************************************/

#ifndef MK_TRANSMITTER_CONFIG_H
#define MK_TRANSMITTER_CONFIG_H

#include <MKShared.h>

namespace MK::TransmitterConfig
{

inline constexpr std::uint32_t TransmitPeriodMs = 50;
inline constexpr bool InputTestMode = false;
inline constexpr Types::MacAddress ReceiverMacAddress  =
{
    0xCC,
    0xDB,
    0xA7,
    0x3E,
    0xD7,
    0x74
};

// inline constexpr Types::MacAddress ReceiverMacAddress  =
// {
//     0x28,
//     0x05,
//     0xA5,
//     0x0B,
//     0x42,
//     0xF8
// };

} // namespace MK::TransmitterConfig

#endif