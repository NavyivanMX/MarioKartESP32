/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : VehicleStatus.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Estado actual del vehículo enviado desde el Receiver hacia Android.
 ******************************************************************************/

#ifndef MK_RECEIVER_VEHICLESTATUS_H
#define MK_RECEIVER_VEHICLESTATUS_H

//=============================================================================
// Includes
//=============================================================================

#include <cstdint>

#include "src/Vehicle/VehicleProfiles/DrivingProfileId.h"

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

#endif // MK_RECEIVER_VEHICLESTATUS_H