#ifndef MK_RECEIVER_DRIVINGPROFILES_H
#define MK_RECEIVER_DRIVINGPROFILES_H

#include "DrivingProfile.h"
#include "DrivingProfileId.h"

namespace MK::VehicleProfiles
{

constexpr std::size_t ProfileCount =
    static_cast<std::size_t>(
        DrivingProfileId::Count);

extern const DrivingProfile Profiles[ProfileCount];

} // namespace MK::VehicleProfiles

#endif