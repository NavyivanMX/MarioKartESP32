/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : VehicleStatusSerializer.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Serialización del VehicleStatus.
 ******************************************************************************/

#ifndef MK_RECEIVER_VEHICLESTATUSSERIALIZER_H
#define MK_RECEIVER_VEHICLESTATUSSERIALIZER_H

//=============================================================================
// Includes
//=============================================================================

#include <cstddef>
#include <cstdint>

#include <MKShared.h>

namespace MK::Protocol
{

//=============================================================================
// Serialización
//=============================================================================

[[nodiscard]]
bool SerializeVehicleStatus(
    const VehicleStatus& status,
    std::uint8_t* buffer,
    std::size_t length) noexcept;

//=============================================================================
// Deserialización
//=============================================================================

[[nodiscard]]
bool DeserializeVehicleStatus(
    const std::uint8_t* buffer,
    std::size_t length,
    VehicleStatus& status) noexcept;

} // namespace MK::Protocol

#endif // MK_RECEIVER_VEHICLESTATUSSERIALIZER_H