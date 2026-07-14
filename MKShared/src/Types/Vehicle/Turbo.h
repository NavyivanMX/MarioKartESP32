/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Turbo.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Define el modo de turbo del vehículo.
 ******************************************************************************/

#ifndef MK_SHARED_TURBO_H
#define MK_SHARED_TURBO_H

#include <cstdint>

namespace MK::Types::Vehicle
{

//=============================================================================
// Turbo
//=============================================================================

//-----------------------------------------------------------------------------
// Estado del turbo
//-----------------------------------------------------------------------------

enum class Turbo : std::uint8_t
{
    Disabled = 0,
    Enabled
};

} // namespace MK::Types::Vehicle

#endif // MK_SHARED_TURBO_H