/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotorController.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador de motores.
 * Recibe el estado deseado para ambos motores y delega
 * la ejecución al HAL.
 ******************************************************************************/

#pragma once

#include "src/Vehicle/MotorState.h"

namespace MK
{

class MotorController
{
public:

    void Initialize() noexcept;

    void Update(
        const MotorState& left,
        const MotorState& right) noexcept;
};

}