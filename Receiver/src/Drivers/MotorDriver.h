/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotorDriver.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Driver de hardware para el sistema de propulsión.
 * Encapsula el control de ambos motores mediante el puente H.
 ******************************************************************************/

#ifndef MK_RECEIVER_MOTORDRIVER_H
#define MK_RECEIVER_MOTORDRIVER_H

#include <MKShared.h>

namespace MK
{

class MotorDriver final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    /// Inicializa el hardware del sistema de propulsión.
    [[nodiscard]]
    bool Begin() noexcept;

    //=========================================================================
    // Control
    //=========================================================================

    /// Detiene ambos motores.
    void Stop() noexcept;

    /// Aplica el estado del motor izquierdo.
    void SetLeftMotor(
        Types::Vehicle::Direction direction,
        Types::Vehicle::Turbo turbo) noexcept;

    /// Aplica el estado del motor derecho.
    void SetRightMotor(
        Types::Vehicle::Direction direction,
        Types::Vehicle::Turbo turbo) noexcept;

private:

    //=========================================================================
    // Implementación
    //=========================================================================

    /// Configura un motor del puente H.
    void ApplyMotor(
        std::uint8_t in1,
        std::uint8_t in2,
        std::uint8_t enable,
        bool invert,
        Types::Vehicle::Direction direction,
        Types::Vehicle::Turbo turbo) noexcept;
};

} // namespace MK

#endif // MK_RECEIVER_MOTORDRIVER_H