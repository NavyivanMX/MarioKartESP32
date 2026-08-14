/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Pins.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Define el mapa de pines del Transmitter.
 ******************************************************************************/

#ifndef MK_PINS_H
#define MK_PINS_H

#include <cstdint>
#include "src/Config/Pins.h"

namespace MK::Pins
{

//=============================================================================
// Botones de dirección
//=============================================================================

inline constexpr std::uint8_t Forward = 13;

inline constexpr std::uint8_t Reverse = 12;

inline constexpr std::uint8_t Left = 14;

inline constexpr std::uint8_t Right = 27;

//=============================================================================
// Botones auxiliares
//=============================================================================

inline constexpr std::uint8_t Turbo = 26;

inline constexpr std::uint8_t Gravity = 25;

inline constexpr std::uint8_t Haptic = 4;

//=============================================================================
// Luces led
//=============================================================================
inline constexpr std::uint8_t StatusLed = 5;

} // namespace MK::Pins

#endif // MK_PINS_H