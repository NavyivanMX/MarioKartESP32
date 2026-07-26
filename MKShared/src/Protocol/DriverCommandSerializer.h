/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DriverCommandSerializer.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Serializa y deserializa DriverCommand para su transmisión entre
 * dispositivos.
 *
 * El formato del paquete es:
 *
 * Byte 0 : Magic Byte 1 ('M')
 * Byte 1 : Magic Byte 2 ('K')
 * Byte 2 : Versión del protocolo
 * Byte 3 : Direction
 * Byte 4 : Steering
 * Byte 5 : Turbo
 * Byte 6 : DriveMode
 * Byte 7 : Checksum (XOR de bytes 0..6)
 ******************************************************************************/

#ifndef MK_SHARED_DRIVER_COMMAND_SERIALIZER_H
#define MK_SHARED_DRIVER_COMMAND_SERIALIZER_H

//=============================================================================
// Includes
//=============================================================================

#include <cstddef>
#include <cstdint>

#include "Protocol.h"

namespace MK::Protocol
{

class DriverCommandSerializer final
{
public:

    //=========================================================================
    // Protocolo
    //=========================================================================

    /// Magic Bytes utilizados para identificar un paquete válido.
    static constexpr std::uint8_t MagicByte1 = 'M';

    static constexpr std::uint8_t MagicByte2 = 'K';

    /// Versión actual del protocolo.
    static constexpr std::uint8_t Version = 1;

    /// Tamaño total del paquete.
    static constexpr std::size_t PacketSize = 8;

    //=========================================================================
    // Índices del paquete
    //=========================================================================

    static constexpr std::size_t MagicByte1Index = 0;

    static constexpr std::size_t MagicByte2Index = 1;

    static constexpr std::size_t VersionIndex = 2;

    static constexpr std::size_t DirectionIndex = 3;

    static constexpr std::size_t SteeringIndex = 4;

    static constexpr std::size_t TurboIndex = 5;

    static constexpr std::size_t DriveModeIndex = 6;

    static constexpr std::size_t ChecksumIndex = 7;

public:

    //=========================================================================
    // Serialización
    //=========================================================================

    /// Convierte un DriverCommand en un paquete binario.
    [[nodiscard]]
    static bool Serialize(
        const DriverCommand& command,
        std::uint8_t* buffer) noexcept;

    /// Reconstruye un DriverCommand a partir de un paquete binario.
    [[nodiscard]]
    static bool Deserialize(
        const std::uint8_t* buffer,
        DriverCommand& command) noexcept;

private:

    //=========================================================================
    // Utilidades
    //=========================================================================

    /// Calcula el checksum XOR.
    [[nodiscard]]
    static std::uint8_t CalculateChecksum(
        const std::uint8_t* buffer,
        std::size_t length) noexcept;
};

} // namespace MK::Protocol

#endif // MK_SHARED_DRIVER_COMMAND_SERIALIZER_H