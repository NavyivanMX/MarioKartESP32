/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DriverCommandFormatter.cpp
 ******************************************************************************/

#include "Utilities/DriverCommandFormatter.h"

#include "Utilities/DriverCommandUtils.h"
#include "Utilities/EnumToString.h"

namespace MK
{
//=============================================================================
// DriverCommandFormatter
//=============================================================================
String DriverCommandFormatter::Format(
    const Protocol::DriverCommand& command)
{
    String result;
    result.reserve(48);

    result += ToString(command.direction);

    if (IsTurning(command))
    {
        result += F(" + ");
        result += ToString(command.steering);
    }

    if (IsTurboEnabled(command))
    {
        result += F(" + Turbo");
    }

    if (IsGravityMode(command))
    {
        result += F(" + Gravity");
    }

    return result;
}

} // namespace MK