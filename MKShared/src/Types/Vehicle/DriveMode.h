/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DriveMode.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Define el modo de conducción del vehículo.
 ******************************************************************************/

#ifndef MK_SHARED_DRIVEMODE_H
#define MK_SHARED_DRIVEMODE_H

#include <cstdint>

namespace MK::Types::Vehicle
{

//=============================================================================
// DriveMode
//=============================================================================

/// Modo de conducción del vehículo.
enum class DriveMode : std::uint8_t
{
    /// Llantas alineadas para conducción convencional.
    Normal = 0,

    /// Llantas orientadas horizontalmente para el modo Gravity.
    Gravity
};


} // namespace MK::Types::Vehicle

#endif // MK_SHARED_DRIVEMODE_H