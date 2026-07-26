/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DriverCommandSerializer.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del serializador de DriverCommand.
 ******************************************************************************/

#include "DriverCommandSerializer.h"

namespace MK::Protocol
{

//=============================================================================
// Serialización
//=============================================================================

bool DriverCommandSerializer::Serialize(
    const DriverCommand& command,
    std::uint8_t* buffer) noexcept
{
    if (buffer == nullptr)
    {
        return false;
    }

    //---------------------------------------------------------------------
    // Encabezado
    //---------------------------------------------------------------------

    buffer[MagicByte1Index] = MagicByte1;
    buffer[MagicByte2Index] = MagicByte2;
    buffer[VersionIndex] = Version;

    //---------------------------------------------------------------------
    // DriverCommand
    //---------------------------------------------------------------------

    buffer[DirectionIndex] =
        static_cast<std::uint8_t>(
            command.direction);

    buffer[SteeringIndex] =
        static_cast<std::uint8_t>(
            command.steering);

    buffer[TurboIndex] =
        static_cast<std::uint8_t>(
            command.turbo);

    buffer[DriveModeIndex] =
        static_cast<std::uint8_t>(
            command.driveMode);

    //---------------------------------------------------------------------
    // Checksum
    //---------------------------------------------------------------------

    buffer[ChecksumIndex] =
        CalculateChecksum(
            buffer,
            ChecksumIndex);

    return true;
}

//=============================================================================
// Deserialización
//=============================================================================

bool DriverCommandSerializer::Deserialize(
    const std::uint8_t* buffer,
    DriverCommand& command) noexcept
{
    if (buffer == nullptr)
    {
        return false;
    }

    //---------------------------------------------------------------------
    // Validar encabezado
    //---------------------------------------------------------------------

    if (buffer[MagicByte1Index] != MagicByte1)
    {
        return false;
    }

    if (buffer[MagicByte2Index] != MagicByte2)
    {
        return false;
    }

    //---------------------------------------------------------------------
    // Validar versión
    //---------------------------------------------------------------------

    if (buffer[VersionIndex] != Version)
    {
        return false;
    }

    //---------------------------------------------------------------------
    // Validar checksum
    //---------------------------------------------------------------------

    if (buffer[ChecksumIndex] !=
        CalculateChecksum(
            buffer,
            ChecksumIndex))
    {
        return false;
    }

    //---------------------------------------------------------------------
    // DriverCommand
    //---------------------------------------------------------------------

    command.direction =
        static_cast<Direction>(
            buffer[DirectionIndex]);

    command.steering =
        static_cast<Steering>(
            buffer[SteeringIndex]);

    command.turbo =
        static_cast<Turbo>(
            buffer[TurboIndex]);

    command.driveMode =
        static_cast<DriveMode>(
            buffer[DriveModeIndex]);

    return true;
}

//=============================================================================
// Utilidades
//=============================================================================

std::uint8_t DriverCommandSerializer::CalculateChecksum(
    const std::uint8_t* buffer,
    std::size_t length) noexcept
{
    std::uint8_t checksum = 0;

    for (std::size_t i = 0; i < length; ++i)
    {
        checksum ^= buffer[i];
    }

    return checksum;
}

} // namespace MK::Protocol