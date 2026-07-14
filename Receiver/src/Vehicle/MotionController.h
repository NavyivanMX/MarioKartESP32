/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotionController.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Controlador del movimiento del vehículo.
 ******************************************************************************/

#pragma once

#include <MKShared.h>

#include "src/Vehicle/MotorController.h"

namespace MK
{

class MotionController
{
public:

    void Initialize() noexcept;

    void Update(
        const Protocol::DriverCommand& command) noexcept;

private:
    void Stop() noexcept;

    void Forward(Types::Vehicle::Turbo turbo) noexcept;

    void Backward(Types::Vehicle::Turbo turbo) noexcept;

    void RotateLeft(
        Types::Vehicle::Turbo turbo) noexcept;

    void RotateRight(
        Types::Vehicle::Turbo turbo) noexcept;

    void ForwardLeft(
        Types::Vehicle::Turbo turbo) noexcept;

    void ForwardRight(
        Types::Vehicle::Turbo turbo) noexcept;

    void BackwardLeft(
        Types::Vehicle::Turbo turbo) noexcept;

    void BackwardRight(
        Types::Vehicle::Turbo turbo) noexcept;

private:

    MotorController m_motor;
};

}