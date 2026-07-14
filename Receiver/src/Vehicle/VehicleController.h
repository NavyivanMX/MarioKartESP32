/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : VehicleController.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador principal del vehículo.
 * Interpreta los comandos recibidos y coordina los diferentes subsistemas
 * del kart.
 ******************************************************************************/

#pragma once

#include <MKShared.h>

#include "src/Vehicle/MotionController.h"
#include "src/Vehicle/GravityController.h"

namespace MK
{

class VehicleController
{
public:

    void Initialize() noexcept;

    void Update(
        const Protocol::DriverCommand& command) noexcept;

private:

    MotionController m_motion;

    GravityController m_gravity;
};

}