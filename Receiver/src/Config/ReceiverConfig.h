/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ReceiverConfig.h
 *
 * Descripción:
 * Configuración específica del vehículo Receiver.
 ******************************************************************************/

#ifndef MK_RECEIVER_CONFIG_H
#define MK_RECEIVER_CONFIG_H

//=============================================================================
// Includes
//=============================================================================

#include <array>

#include <MKShared.h>

namespace MK::ReceiverConfig
{

//=============================================================================
// Desarrollo
//=============================================================================

inline constexpr bool HardwareTestMode = true;

//=============================================================================
// Comunicación
//=============================================================================

inline constexpr Types::MacAddress
OriginalTransmitter =
{
    0xCC,
    0xDB,
    0xA7,
    0x3D,
    0xA9,
    0xF0
};

inline constexpr Types::MacAddress
LaboratoryTransmitter =
{
    0xC0,
    0x49,
    0xEF,
    0xE4,
    0x72,
    0x3C
};

//-----------------------------------------------------------------------------
// Transmisores autorizados
//-----------------------------------------------------------------------------

inline constexpr std::array<Types::MacAddress, 2>
AuthorizedTransmitters
{
    LaboratoryTransmitter,
    OriginalTransmitter
};

//-----------------------------------------------------------------------------
// Validación de MAC
//-----------------------------------------------------------------------------

inline constexpr bool ValidateTransmitterMac = false;

//-----------------------------------------------------------------------------
// Timeout de comunicación
//-----------------------------------------------------------------------------

inline constexpr std::uint32_t CommandTimeoutMs = 500;

//=============================================================================
// Utilidades
//=============================================================================

[[nodiscard]]
inline constexpr bool IsAuthorizedTransmitter(
    const Types::MacAddress& mac) noexcept
{
    for (const auto& authorized :
    AuthorizedTransmitters)
    {
        if (authorized == mac)
        {
            return true;
        }
    }

    return false;
}

} // namespace MK::ReceiverConfig

#endif