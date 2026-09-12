/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : TransmitterConfig.h
 *
 * Descripción:
 * Configuración específica del Transmitter.
 ******************************************************************************/

#ifndef MK_TRANSMITTER_CONFIG_H
#define MK_TRANSMITTER_CONFIG_H

#include <MKShared.h>

namespace MK::TransmitterConfig
{

//=============================================================================
// Configuración general
//=============================================================================

inline constexpr std::uint32_t TransmitPeriodMs = 50;

inline constexpr bool InputTestMode = false;


//=============================================================================
// Receptores ESP-NOW
//
// El orden es importante:
//
//   0 = Kart
//   1 = Laboratorio
//
// El Transmitter intenta primero con Kart.
// Si no responde, intenta con Laboratorio.
//
// Una vez seleccionado un receptor, NO cambia durante la sesión.
// Para cambiar de receptor es necesario reiniciar el Transmitter.
//=============================================================================

//-----------------------------------------------------------------------------
// MAC del Kart
//-----------------------------------------------------------------------------
inline constexpr Types::MacAddress ReceiverMacAddressKart =
{
    0x28,
    0x05,
    0xA5,
    0x0B,
    0x42,
    0xF8
};


//-----------------------------------------------------------------------------
// MAC del ESP32 de laboratorio
//
// TODO:
// Reemplazar estos valores por la MAC real del ESP32 de laboratorio.
//-----------------------------------------------------------------------------
inline constexpr Types::MacAddress ReceiverMacAddressLab =
{
    0xCC,
    0xDB,
    0xA7,
    0x3E,
    0xD7,
    0x74
};

} // namespace MK::TransmitterConfig

#endif // MK_TRANSMITTER_CONFIG_H