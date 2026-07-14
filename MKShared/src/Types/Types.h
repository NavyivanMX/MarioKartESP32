/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Types.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Define los tipos fundamentales compartidos por todo el proyecto.
 ******************************************************************************/

#ifndef MK_SHARED_TYPES_H
#define MK_SHARED_TYPES_H

#include <array>
#include <cstddef>
#include <cstdint>


namespace MK::Types
{

//=============================================================================
// Alias
//=============================================================================

/// Longitud de una dirección MAC.
inline constexpr std::size_t MacAddressLength = 6;

/// Tipo que representa una dirección MAC.
using MacAddress = std::array<std::uint8_t, MacAddressLength>;

} // namespace MK::Types

#endif // MK_SHARED_TYPES_H