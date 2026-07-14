/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Button.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Encapsula la lectura de un botón físico.
 ******************************************************************************/

#ifndef MK_SHARED_BUTTON_H
#define MK_SHARED_BUTTON_H

#include <cstdint>

namespace MK
{

class Button final
{
public:

    explicit Button(std::uint8_t pin);

    void Begin() noexcept;

    void Update() noexcept;

    [[nodiscard]]
    bool IsPressed() const noexcept;

    [[nodiscard]]
    bool WasPressed() const noexcept;

    [[nodiscard]]
    bool WasReleased() const noexcept;

private:

    std::uint8_t m_pin = 0;

    bool m_pressed = false;

    bool m_previousPressed = false;
};

} // namespace MK

#endif // MK_SHARED_BUTTON_H