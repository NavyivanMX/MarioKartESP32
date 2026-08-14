/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DrivingProfiles.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Define la colección de perfiles de conducción del Receiver.
 ******************************************************************************/

#ifndef MK_RECEIVER_DRIVINGPROFILES_H
#define MK_RECEIVER_DRIVINGPROFILES_H

//=============================================================================
// Includes
//=============================================================================

#include <cstddef>

#include <MKShared.h>

#include "DrivingProfile.h"

namespace MK::VehicleProfiles
{

//=============================================================================
// Configuración
//=============================================================================

constexpr std::size_t ProfileCount =
    static_cast<std::size_t>(
        DrivingProfileId::Count);

//=============================================================================
// Perfiles
//=============================================================================

extern const DrivingProfile Profiles[
    ProfileCount];

} // namespace MK::VehicleProfiles

#endif // MK_RECEIVER_DRIVINGPROFILES_H