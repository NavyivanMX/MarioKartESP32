/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DriverCommand.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Define el comando de conducción enviado desde el Transmitter hacia el
 * Receiver.
 *
 * Esta estructura representa el Payload del paquete DriverCommand y no
 * contiene lógica de serialización ni comportamiento adicional.
 ******************************************************************************/

#ifndef MK_SHARED_DRIVERCOMMAND_H
#define MK_SHARED_DRIVERCOMMAND_H

//=============================================================================
// Includes
//=============================================================================

#include "Types/Vehicle/Direction.h"
#include "Types/Vehicle/Steering.h"
#include "Types/Vehicle/Turbo.h"
#include "Types/Vehicle/DriveMode.h"

namespace MK::Protocol
{

//=============================================================================
// DriverCommand
//=============================================================================

struct DriverCommand
{
    //-------------------------------------------------------------------------
    // Movimiento longitudinal
    //-------------------------------------------------------------------------

    Types::Vehicle::Direction direction{
        Types::Vehicle::Direction::Stop};

    //-------------------------------------------------------------------------
    // Dirección
    //-------------------------------------------------------------------------

    Types::Vehicle::Steering steering{
        Types::Vehicle::Steering::Straight};

    //-------------------------------------------------------------------------
    // Turbo
    //-------------------------------------------------------------------------

    Types::Vehicle::Turbo turbo{
        Types::Vehicle::Turbo::Disabled};

    //-------------------------------------------------------------------------
    // Perfil de conducción
    //-------------------------------------------------------------------------

    Types::Vehicle::DriveMode driveMode{
        Types::Vehicle::DriveMode::Normal};

        [[nodiscard]]
constexpr bool operator==(
    const DriverCommand& other) const noexcept
{
    return
        direction == other.direction &&
        steering == other.steering &&
        turbo == other.turbo &&
        driveMode == other.driveMode;
}

[[nodiscard]]
constexpr bool operator!=(
    const DriverCommand& other) const noexcept
{
    return !(*this == other);
}
};

} // namespace MK::Protocol

#endif // MK_SHARED_DRIVERCOMMAND_H