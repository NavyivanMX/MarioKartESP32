#pragma once

#include <MKShared.h>

namespace MK
{

enum class MotorDirection
{
    Stop,
    Forward,
    Reverse
};

struct MotorState
{
    MotorDirection direction;

    Types::Vehicle::Turbo turbo;
};

}