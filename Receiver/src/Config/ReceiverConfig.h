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
    0xFF// 0xF0 esta es la original, la que se usó en el proyecto de MarioKart ESP32 RC
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

inline constexpr Types::MacAddress
MarioTransmitter =
{
    0xC8,
    0x8A,
    0x7B,
    0x8B,
    0x78,
    0x54
};


//-----------------------------------------------------------------------------
// Transmisores autorizados
//-----------------------------------------------------------------------------

inline constexpr std::array<Types::MacAddress, 3>
AuthorizedTransmitters
{
    LaboratoryTransmitter,
    OriginalTransmitter,
    MarioTransmitter
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