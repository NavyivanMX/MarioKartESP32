/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : Button.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación de la clase Button.
 ******************************************************************************/

#include "Utilities/Button.h"

#include <Arduino.h>

namespace MK
{

//=============================================================================
// Constructor
//=============================================================================

Button::Button(std::uint8_t pin)
    : m_pin(pin)
{
}

//=============================================================================
// Inicialización
//=============================================================================

void Button::Begin() noexcept
{
    pinMode(m_pin, INPUT_PULLUP);

    const bool pressed = (digitalRead(m_pin) == LOW);

    m_pressed = pressed;
    m_previousPressed = pressed;
}

//=============================================================================
// Actualización
//=============================================================================

void Button::Update() noexcept
{
    m_previousPressed = m_pressed;

    m_pressed = (digitalRead(m_pin) == LOW);
}

//=============================================================================
// Estado actual
//=============================================================================

bool Button::IsPressed() const noexcept
{
    return m_pressed;
}

//=============================================================================
// Flanco de subida
//=============================================================================

bool Button::WasPressed() const noexcept
{
    return (!m_previousPressed && m_pressed);
}

//=============================================================================
// Flanco de bajada
//=============================================================================

bool Button::WasReleased() const noexcept
{
    return (m_previousPressed && !m_pressed);
}

} // namespace MK