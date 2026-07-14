#pragma once

#include "MotorState.h"

namespace MK
{

inline const char* ToString(MotorDirection dir)
{
    switch (dir)
    {
        case MotorDirection::Stop:
            return "Stop";

        case MotorDirection::Forward:
            return "Forward";

        case MotorDirection::Reverse:
            return "Reverse";
    }

    return "Unknown";
}

}