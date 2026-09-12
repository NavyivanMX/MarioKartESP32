/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Pins.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Mapa de pines del Receiver.
 * Centraliza la asignación de GPIO utilizados por los actuadores del vehículo.
 ******************************************************************************/

#ifndef MK_RECEIVER_PINS_H
#define MK_RECEIVER_PINS_H

#include <cstdint>

namespace MK::Pins
{

//=============================================================================
// Motor izquierdo
//=============================================================================

inline constexpr std::uint8_t LeftMotorIn1 = 26;

inline constexpr std::uint8_t LeftMotorIn2 = 27;

inline constexpr std::uint8_t LeftMotorPwm = 25;

//=============================================================================
// Motor derecho
//=============================================================================

inline constexpr std::uint8_t RightMotorIn1 = 33;

inline constexpr std::uint8_t RightMotorIn2 = 32;

inline constexpr std::uint8_t RightMotorPwm = 14;
//=============================================================================
// Ambiente - WS2812B
//=============================================================================
inline constexpr std::uint8_t AmbientLights = 19;

//=============================================================================
// Estado del turbo
//=============================================================================
inline constexpr std::uint8_t Haptic = 4;























//=============================================================================
// Modo Gravity
//=============================================================================

inline constexpr std::uint8_t Gravity = 13;

} // namespace MK::Pins

#endif // MK_RECEIVER_PINS_H