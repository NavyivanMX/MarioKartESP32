/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotionController.h
 ******************************************************************************/

#ifndef MK_RECEIVER_MOTIONCONTROLLER_H
#define MK_RECEIVER_MOTIONCONTROLLER_H

#include <MKShared.h>

#include "src/Vehicle/MotorController.h"
#include "src/Vehicle/VehicleProfiles/DrivingProfile.h"

namespace MK
{

class MotionController final
{
public:

    [[nodiscard]]
    bool Begin() noexcept;

    void Update(
        const Protocol::DriverCommand& command,
        const VehicleProfiles::DrivingProfile& profile) noexcept;
    void Stop() noexcept;
private:

    MotorController m_motor;
};

}

#endif // MK_RECEIVER_MOTIONCONTROLLER_H