/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotorController.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador principal del vehículo.
 * Interpreta los comandos recibidos y coordina los diferentes subsistemas
 * del kart.
 ******************************************************************************/
#pragma once

#include <MKShared.h>

#include "src/Vehicle/MotorController.h"
#include "src/Vehicle/GravityController.h"

namespace MK
{
    class GravityController
    {
    public:

        void Initialize() noexcept;

        void Enable() noexcept;

        void Disable() noexcept;

    private:

        bool m_enabled = false;
    };
}