/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Config.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Configuración general del firmware.
 ******************************************************************************/

#ifndef MK_SHARED_CONFIG_H
#define MK_SHARED_CONFIG_H

#include <cstdint>

namespace MK::Config
{

namespace System
{

/// Periodo principal del loop.
inline constexpr std::uint32_t LoopPeriodMs = 10;

} // namespace System

namespace Communication
{

/// Periodo entre paquetes enviados.
inline constexpr std::uint32_t PacketPeriodMs = 20;

/// Tiempo máximo sin paquetes antes de detener el vehículo.
inline constexpr std::uint32_t PacketTimeoutMs = 250;

} // namespace Communication

namespace Debug
{

inline constexpr bool EnableSerial = true;

inline constexpr bool OnlyOnChange = true;

} // namespace Debug

} // namespace MK::Config
#endif // MK_SHARED_CONFIG_H