/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Button.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Encapsula la lectura de un botón físico con debounce por software.
 ******************************************************************************/

#ifndef MK_BUTTON_H
#define MK_BUTTON_H

#include <Arduino.h>

#include <cstdint>

namespace MK
{

class Button final
{
public:

    explicit Button(std::uint8_t pin);

    void Begin();

    void Update();

    [[nodiscard]]
    bool IsPressed() const noexcept;

    [[nodiscard]]
    bool WasPressed() const noexcept;

    [[nodiscard]]
    bool WasReleased() const noexcept;

private:

    std::uint8_t m_pin;

    bool m_pressed = false;

    bool m_previousPressed = false;
};

} // namespace MK

#endif // MK_BUTTON_H