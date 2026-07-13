/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DriverCommandFormatter.h
 ******************************************************************************/

#ifndef MK_SHARED_DRIVERCOMMANDFORMATTER_H
#define MK_SHARED_DRIVERCOMMANDFORMATTER_H

#include <Arduino.h>

#include "Protocol/Protocol.h"

namespace MK::Protocol
{

class DriverCommandFormatter final
{
public:

    DriverCommandFormatter() = delete;

    [[nodiscard]]
    static String Format(const Protocol::DriverCommand& command);
};

} // namespace MK

#endif // MK_SHARED_DRIVERCOMMANDFORMATTER_H