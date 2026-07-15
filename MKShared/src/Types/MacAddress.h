/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MacAddress.h
 *
 * Descripción:
 * Define el tipo utilizado para representar una dirección MAC.
 ******************************************************************************/

#ifndef MK_SHARED_MACADDRESS_H
#define MK_SHARED_MACADDRESS_H

#include <array>
#include <cstddef>
#include <cstdint>

namespace MK::Types
{

//=============================================================================
// Dirección MAC
//=============================================================================

inline constexpr std::size_t MacAddressLength = 6;

/// Tipo que representa una dirección MAC.
using MacAddress = std::array<std::uint8_t, MacAddressLength>;

} // namespace MK::Types

#endif // MK_SHARED_MACADDRESS_H