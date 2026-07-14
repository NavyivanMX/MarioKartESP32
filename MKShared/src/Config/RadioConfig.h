/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : RadioConfig.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Configuración compartida de comunicación inalámbrica (ESP-NOW).
 ******************************************************************************/

#ifndef MK_SHARED_RADIO_CONFIG_H
#define MK_SHARED_RADIO_CONFIG_H

#include <cstdint>

namespace MK::RadioConfig
{

//=============================================================================
// Parámetros de comunicación ESP-NOW
//=============================================================================

/// Canal WiFi utilizado por ESP-NOW.
/// Ambos dispositivos deben utilizar exactamente el mismo canal.
inline constexpr std::uint8_t Channel = 1;

/// Indica si ESP-NOW utilizará cifrado.
/// Actualmente se encuentra deshabilitado.
inline constexpr bool Encryption = false;

} // namespace MK::RadioConfig

#endif // MK_SHARED_RADIO_CONFIG_H