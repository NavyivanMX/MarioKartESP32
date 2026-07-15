/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ReceiverController.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Orquestador principal del firmware Receiver.
 * Coordina la comunicación inalámbrica, la depuración y el control
 * del vehículo.
 ******************************************************************************/

#ifndef MK_RECEIVER_CONTROLLER_H
#define MK_RECEIVER_CONTROLLER_H

#include "src/Communication/ESPNowReceiver.h"
#include "src/Debug/ConsoleLogger.h"
#include "src/Vehicle/VehicleController.h"

namespace MK
{

class ReceiverController final
{
public:

    /// Inicializa todos los módulos del receptor.
    [[nodiscard]]
    bool Begin() noexcept;

    /// Ejecuta un ciclo de actualización.
    void Update() noexcept;

private:
    //=========================================================================
    // Depuración
    //=========================================================================

    ConsoleLogger m_logger;
    
    //=========================================================================
    // Comunicación
    //=========================================================================

    ESPNowReceiver m_receiver;

    //=========================================================================
    // Vehículo
    //=========================================================================

    VehicleController m_vehicle;


};

} // namespace MK

#endif // MK_RECEIVER_CONTROLLER_H