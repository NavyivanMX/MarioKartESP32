/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : GravityDriver.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Driver del mecanismo de cambio entre los modos de conducción Normal y
 * Gravity.
 ******************************************************************************/

#ifndef MK_RECEIVER_GRAVITYDRIVER_H
#define MK_RECEIVER_GRAVITYDRIVER_H

#include <MKShared.h>

namespace MK
{

class GravityDriver
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    [[nodiscard]]
    bool Begin() noexcept;

    //=========================================================================
    // Control
    //=========================================================================

    /// Configura el modo de conducción.
    void SetMode(
        Types::Vehicle::DriveMode mode) noexcept;
};

} // namespace MK

#endif // MK_RECEIVER_GRAVITYDRIVER_H