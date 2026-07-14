/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ReceiverController.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Orquestador principal del receptor.
 * Coordina la comunicación ESP-NOW, el procesamiento de comandos
 * y el control del vehículo.
 ******************************************************************************/

#pragma once

#include "src/Communication/ESPNowReceiver.h"
#include "src/Vehicle/VehicleController.h"

namespace MK
{

class ReceiverController
{
public:

    void Initialize() noexcept;

    void Update() noexcept;

private:

    ESPNowReceiver m_receiver;

    VehicleController m_vehicle;
};

}