/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DrivingProfileStorage.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Persistencia del perfil de conducción utilizando NVS.
 ******************************************************************************/

#ifndef MK_RECEIVER_DRIVINGPROFILESTORAGE_H
#define MK_RECEIVER_DRIVINGPROFILESTORAGE_H

//=============================================================================
// Includes
//=============================================================================

#include <MKShared.h>

namespace MK::VehicleProfiles
{

class DrivingProfileStorage final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    bool Begin() noexcept;

    //=========================================================================
    // Persistencia
    //=========================================================================

    DrivingProfileId Load() noexcept;

    void Save(
        DrivingProfileId profile) noexcept;
};

} // namespace MK::VehicleProfiles

#endif