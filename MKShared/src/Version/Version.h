/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Version.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Información de versión del firmware.
 ******************************************************************************/

#ifndef MK_SHARED_VERSION_H
#define MK_SHARED_VERSION_H

#include <cstdint>

namespace MK::Version
{

//=============================================================================
// Firmware
//=============================================================================

inline constexpr std::uint8_t Major = 1;
inline constexpr std::uint8_t Minor = 1;
inline constexpr std::uint8_t Patch = 0;

//=============================================================================
// Compilación
//=============================================================================

inline constexpr char Name[] = "MarioKart RC";
inline constexpr char Project[] = "MarioKart ESP32 RC";
inline constexpr char Release[] = "RC1.1";
inline constexpr char Author[] = "Narciso Ivan Cisneros Acosta";


} // namespace MK::Version

#endif // MK_SHARED_VERSION_H