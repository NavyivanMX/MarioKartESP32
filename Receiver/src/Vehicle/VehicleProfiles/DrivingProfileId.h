/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DrivingProfileId.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Identificadores de los perfiles de conducción.
 ******************************************************************************/

#ifndef MK_RECEIVER_DRIVINGPROFILEID_H
#define MK_RECEIVER_DRIVINGPROFILEID_H

#include <cstdint>

namespace MK::VehicleProfiles
{

enum class DrivingProfileId : std::uint8_t
{
    Rookie = 0,

    Normal,

    Advanced,

    Drift,

    Count
};

} // namespace MK::VehicleProfiles

#endif // MK_RECEIVER_DRIVINGPROFILEID_H