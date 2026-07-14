/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Version.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Información de versión de la librería MKShared y del proyecto.
 ******************************************************************************/

#ifndef MK_SHARED_VERSION_H
#define MK_SHARED_VERSION_H

#include <cstdint>

namespace MK::Version
{

//=============================================================================
// Versión
//=============================================================================

inline constexpr std::uint8_t Major = 1;
inline constexpr std::uint8_t Minor = 1;
inline constexpr std::uint8_t Patch = 0;

//=============================================================================
// Información de la librería
//=============================================================================

inline constexpr char Library[] = "MKShared";

//=============================================================================
// Información del proyecto
//=============================================================================

inline constexpr char ProjectName[] = "MarioKart ESP32 RC";

inline constexpr char Edition[] = "RC1.1";

inline constexpr char Author[] = "Narciso Ivan Cisneros Acosta";

inline constexpr char Copyright[] =
    "Copyright (c) 2026 Narciso Ivan Cisneros Acosta";


} // namespace MK::Version

#endif // MK_SHARED_VERSION_H