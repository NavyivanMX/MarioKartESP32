/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : VehicleStatusSerializer.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación de la serialización de VehicleStatus.
 ******************************************************************************/

#include "VehicleStatusSerializer.h"

#include <cstring>

namespace MK::Protocol
{

//=============================================================================
// Serialización
//=============================================================================

bool SerializeVehicleStatus(
    const VehicleStatus& status,
    std::uint8_t* buffer,
    std::size_t length) noexcept
{
    if (buffer == nullptr)
    {
        return false;
    }

    constexpr std::size_t PayloadSize =
        sizeof(VehicleStatus);

    if (length < PayloadSize)
    {
        return false;
    }

    std::memcpy(
        buffer,
        &status,
        PayloadSize);

    return true;
}

//=============================================================================
// Deserialización
//=============================================================================

bool DeserializeVehicleStatus(
    const std::uint8_t* buffer,
    std::size_t length,
    VehicleStatus& status) noexcept
{
    if (buffer == nullptr)
    {
        return false;
    }

    constexpr std::size_t PayloadSize =
        sizeof(VehicleStatus);

    if (length < PayloadSize)
    {
        return false;
    }

    std::memcpy(
        &status,
        buffer,
        PayloadSize);

    return true;
}

} // namespace MK::Protocol