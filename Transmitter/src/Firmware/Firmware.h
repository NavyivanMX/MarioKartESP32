/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Firmware.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Información del firmware Transmitter.
 ******************************************************************************/

#ifndef MK_TRANSMITTER_FIRMWARE_H
#define MK_TRANSMITTER_FIRMWARE_H

#include <cstdint>

namespace MK::Firmware
{

//=============================================================================
// Versión
//=============================================================================

inline constexpr std::uint8_t Major = 1;
inline constexpr std::uint8_t Minor = 1;
inline constexpr std::uint8_t Patch = 0;

inline constexpr char VersionString[] = "1.1.0";

//=============================================================================
// Información
//=============================================================================

inline constexpr char ProjectName[]  = "MarioKart ESP32 RC";
inline constexpr char FirmwareName[] = "Transmitter";
inline constexpr char Release[]      = "RC1.1";
inline constexpr char Author[]       = "Narciso Ivan Cisneros Acosta";

} // namespace MK::Firmware

#endif // MK_TRANSMITTER_FIRMWARE_H