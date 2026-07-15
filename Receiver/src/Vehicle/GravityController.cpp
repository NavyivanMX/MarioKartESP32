/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : GravityController.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación del controlador del modo Gravity.
 ******************************************************************************/

#include "src/Vehicle/GravityController.h"

namespace MK
{

//=============================================================================
// Ciclo de vida
//=============================================================================

bool GravityController::Begin() noexcept
{
    return m_driver.Begin();
}

//=============================================================================
// Control
//=============================================================================

void GravityController::Update(
    Types::Vehicle::DriveMode mode) noexcept
{
    switch (mode)
    {
        case Types::Vehicle::DriveMode::Normal:
        case Types::Vehicle::DriveMode::Gravity:

            m_driver.SetMode(mode);
            break;

        default:

            // Cualquier modo desconocido vuelve al modo normal
            // por seguridad.
            m_driver.SetMode(
                Types::Vehicle::DriveMode::Normal);

            break;
    }
}

} // namespace MK