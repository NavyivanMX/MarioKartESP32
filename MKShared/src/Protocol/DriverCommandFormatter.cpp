/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DriverCommandFormatter.cpp
 ******************************************************************************/

#include "src/Shared/DriverCommandFormatter.h"

#include "src/Shared/EnumToString.h"
#include "src/Shared/ProtocolUtils.h"

namespace MK
{

String DriverCommandFormatter::Format(
    const Protocol::DriverCommand& command)
{
    String result;

    result += ToString(command.direction);

    if (Protocol::IsTurning(command))
    {
        result += F(" + ");
        result += ToString(command.steering);
    }

    if (Protocol::IsTurboEnabled(command))
    {
        result += F(" + Turbo");
    }

    if (Protocol::IsGravityMode(command))
    {
        result += F(" + Gravity");
    }

    return result;
}

} // namespace MK