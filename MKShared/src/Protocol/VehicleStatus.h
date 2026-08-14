/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : VehicleStatus.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Estado actual del vehículo enviado desde el Receiver hacia Android.
 ******************************************************************************/

#ifndef MK_SHARED_VEHICLE_STATUS_H
#define MK_SHARED_VEHICLE_STATUS_H

//=============================================================================
// Includes
//=============================================================================

#include <cstdint>


#include "Vehicle/VehicleProfiles/DrivingProfileId.h"

namespace MK::Protocol
{

//=============================================================================
// VehicleStatus
//=============================================================================

struct VehicleStatus
{
    //---------------------------------------------------------------------
    // Perfil de conducción actualmente activo.
    //---------------------------------------------------------------------

    VehicleProfiles::DrivingProfileId drivingProfile;
};


} // namespace MK::Protocol

#endif // MK_SHARED_VEHICLE_STATUS_H