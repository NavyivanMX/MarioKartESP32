/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : MotorDriver.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Driver de hardware para el sistema de propulsión.
 *
 * Encapsula el control de ambos motores mediante un puente H TB6612FNG.
 * Su única responsabilidad es traducir el estado calculado por el
 * MotorController (dirección y potencia) a señales físicas del hardware.
 ******************************************************************************/

#ifndef MK_RECEIVER_MOTORDRIVER_H
#define MK_RECEIVER_MOTORDRIVER_H

//=============================================================================
// Includes
//=============================================================================

#include <cstdint>

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

    /// Actualiza el estado del motor izquierdo.
    ///
    /// @param direction Dirección de giro.
    /// @param power     Potencia aplicada (0-255).
    void SetLeftMotor(
        Types::Vehicle::Direction direction,
        std::uint8_t power) noexcept;

    /// Actualiza el estado del motor derecho.
    ///
    /// @param direction Dirección de giro.
    /// @param power     Potencia aplicada (0-255).
    void SetRightMotor(
        Types::Vehicle::Direction direction,
        std::uint8_t power) noexcept;

private:

    //=========================================================================
    // Implementación
    //=========================================================================

    /// Aplica el estado de un motor al TB6612FNG.
    ///
    /// @param in1       Pin IN1 del puente H.
    /// @param in2       Pin IN2 del puente H.
    /// @param pwmPin    GPIO utilizado para PWM (LEDC).
    /// @param invert    Invierte el sentido de giro del motor.
    /// @param direction Dirección solicitada.
    /// @param power     Potencia solicitada (0-255).
    void ApplyMotor(
        std::uint8_t in1,
        std::uint8_t in2,
        std::uint8_t pwmPin,
        bool invert,
        Types::Vehicle::Direction direction,
        std::uint8_t power) noexcept;
};

} // namespace MK

#endif // MK_RECEIVER_MOTORDRIVER_H