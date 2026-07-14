/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DriverCommandFormatter.h
 *
 * Descripción:
 * Convierte un DriverCommand en una cadena legible para depuración.
 ******************************************************************************/

#ifndef MK_SHARED_DRIVERCOMMANDFORMATTER_H
#define MK_SHARED_DRIVERCOMMANDFORMATTER_H

#include <Arduino.h>

#include "Protocol/Protocol.h"

namespace MK
{

//=============================================================================
// DriverCommandFormatter
//=============================================================================

class DriverCommandFormatter final
{
public:

    DriverCommandFormatter() = delete;

    /// Convierte un DriverCommand en una cadena legible.
    [[nodiscard]]
    static String Format(const Protocol::DriverCommand& command);
};

} // namespace MK

#endif // MK_SHARED_DRIVERCOMMANDFORMATTER_H