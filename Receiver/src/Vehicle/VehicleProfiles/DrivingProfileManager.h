/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DrivingProfileManager.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Administra el perfil de conducción actualmente seleccionado.
 ******************************************************************************/

#ifndef MK_RECEIVER_DRIVINGPROFILEMANAGER_H
#define MK_RECEIVER_DRIVINGPROFILEMANAGER_H

//=============================================================================
// Includes
//=============================================================================

#include "DrivingProfiles.h"
#include "DrivingProfileStorage.h"

namespace MK::VehicleProfiles
{

class DrivingProfileManager final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    [[nodiscard]]
    bool Begin() noexcept;

    //=========================================================================
    // Perfil actual
    //=========================================================================

    [[nodiscard]]
    const DrivingProfile&
    Current() const noexcept;

    [[nodiscard]]
    DrivingProfileId
    CurrentId() const noexcept;

    [[nodiscard]]
    const char*
    CurrentName() const noexcept;

    //=========================================================================
    // Navegación
    //=========================================================================

    void Next() noexcept;

    void Previous() noexcept;

    void Set(
        DrivingProfileId profile) noexcept;

    //=========================================================================
    // Notificación
    //=========================================================================

    [[nodiscard]]
    bool ProfileChanged() const noexcept;

    void ClearProfileChanged() noexcept;

private:

    DrivingProfileStorage m_storage;

    DrivingProfileId m_currentId{
        DrivingProfileId::Normal};

    bool m_profileChanged{false};
};

}

#endif