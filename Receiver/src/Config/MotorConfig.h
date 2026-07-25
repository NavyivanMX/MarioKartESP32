/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotorConfig.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Configuración del sistema de propulsión.
 *
 * Centraliza todos los parámetros relacionados con:
 *
 *  - PWM (LEDC)
 *  - Niveles de potencia
 *  - Configuración de dirección
 *  - Configuración mecánica
 ******************************************************************************/

#ifndef MK_RECEIVER_MOTORCONFIG_H
#define MK_RECEIVER_MOTORCONFIG_H

#include <cstdint>

namespace MK::MotorConfig
{

//=============================================================================
// PWM (LEDC)
//=============================================================================

/// Frecuencia del PWM.
///
/// 20 kHz elimina prácticamente todo el ruido audible del motor.
inline constexpr std::uint32_t Frequency = 20000;

/// Resolución del PWM.
///
/// 8 bits = rango de 0 a 255.
inline constexpr std::uint8_t Resolution = 8;

//=============================================================================
// Potencia
//=============================================================================

namespace Power
{

/// Motor detenido.
inline constexpr std::uint8_t Stop = 0;

/// Potencia utilizada para conducción normal.
inline constexpr std::uint8_t Cruise = 180;

/// Potencia máxima (Turbo).
inline constexpr std::uint8_t Turbo = 255;

} // namespace Power

//=============================================================================
// Dirección
//=============================================================================

namespace Steering
{

/// Relación de potencia aplicada a la rueda interior
/// durante un giro en movimiento.
///
/// 1.0 = ambas ruedas misma velocidad.
/// 0.5 = rueda interior al 50 %.
/// 0.0 = rueda interior detenida.
inline constexpr float TurnRatio = 0.50f;

/// Habilita el giro sobre el propio eje (Pivot Turn).
///
/// false -> el vehículo no gira si está detenido.
/// true  -> izquierda/derecha hacen girar el kart como un tanque.
inline constexpr bool PivotTurnEnabled = true;

} // namespace Steering

//=============================================================================
// Configuración mecánica
//=============================================================================

/// Invierte el sentido del motor izquierdo.
inline constexpr bool InvertLeftMotor = false;

/// Invierte el sentido del motor derecho.
inline constexpr bool InvertRightMotor = false;

} // namespace MK::MotorConfig

#endif // MK_RECEIVER_MOTORCONFIG_H