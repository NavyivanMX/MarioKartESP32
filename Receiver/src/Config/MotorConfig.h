/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotorConfig.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Configuración del sistema de propulsión.
 * Centraliza los parámetros del hardware relacionados con los motores.
 ******************************************************************************/

#ifndef MK_RECEIVER_MOTORCONFIG_H
#define MK_RECEIVER_MOTORCONFIG_H

#include <cstdint>

namespace MK::MotorConfig
{

//=============================================================================
// Inversión de motores
//=============================================================================

/// Invierte el sentido del motor izquierdo.
inline constexpr bool InvertLeftMotor = false;

/// Invierte el sentido del motor derecho.
inline constexpr bool InvertRightMotor = false;

//=============================================================================
// PWM
//=============================================================================

namespace Pwm
{

/// Ciclo útil para velocidad normal.
inline constexpr std::uint8_t Normal = 180;

/// Ciclo útil para modo Turbo.
inline constexpr std::uint8_t Turbo = 255;

} // namespace Pwm

} // namespace MK::MotorConfig

#endif // MK_RECEIVER_MOTORCONFIG_H