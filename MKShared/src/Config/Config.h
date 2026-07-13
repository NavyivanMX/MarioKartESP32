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
namespace Debug
{
    inline constexpr bool EnableSerial = true;

    inline constexpr bool OnlyOnChange = true;
}


namespace MK::Config
{
//=============================================================================
// Build
//=============================================================================

namespace Build
{
    /// Habilita el modo de prueba de entradas.
    ///
    /// true  -> No envía paquetes ESP-NOW.
    ///          Sólo muestra los comandos generados por InputManager.
    ///
    /// false -> Funcionamiento normal.
    inline constexpr bool InputTestMode = true;
}
//=============================================================================
// Sistema
//=============================================================================

namespace System
{

/// Periodo principal del loop (ms).
inline constexpr std::uint32_t LoopPeriodMs = 10;

} // namespace System

} // namespace MK::Config

#endif // MK_SHARED_CONFIG_H