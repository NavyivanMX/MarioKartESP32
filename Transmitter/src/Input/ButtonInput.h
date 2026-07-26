/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ButtonInput.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación de una fuente de entrada basada en botones físicos.
 * Lee el estado de todos los botones del transmisor y construye el
 * DriverCommand correspondiente.
 ******************************************************************************/

#ifndef MK_BUTTON_INPUT_H
#define MK_BUTTON_INPUT_H

//=============================================================================
// Includes
//=============================================================================

#include <MKShared.h>

#include "src/Config/Pins.h"
#include "src/Input/IInputSource.h"

namespace MK
{

class ButtonInput final :
    public IInputSource
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    /// Inicializa todos los botones.
    void Begin() noexcept override;

    /// Actualiza el estado de los botones.
    ///
    /// Devuelve true únicamente cuando cambia el DriverCommand.
    [[nodiscard]]
    bool Update() noexcept override;

    //=========================================================================
    // Acceso
    //=========================================================================

    [[nodiscard]]
    const Protocol::DriverCommand&
    GetDriverCommand() const noexcept override;

private:

    //=========================================================================
    // Actualización
    //=========================================================================

    /// Actualiza el estado de todos los botones.
    void ReadButtons() noexcept;

    /// Construye el DriverCommand a partir del estado actual.
    ///
    /// Devuelve true cuando el comando cambia.
    [[nodiscard]]
    bool BuildDriverCommand() noexcept;

    /// Ejecuta una operación sobre todos los botones.
    template<typename Func>
    void ForEachButton(Func&& func);

private:

    //=========================================================================
    // Hardware
    //=========================================================================

    Button m_forwardButton{Pins::Forward};

    Button m_reverseButton{Pins::Reverse};

    Button m_leftButton{Pins::Left};

    Button m_rightButton{Pins::Right};

    Button m_turboButton{Pins::Turbo};

    Button m_gravityButton{Pins::Gravity};

    //=========================================================================
    // Estado
    //=========================================================================

    Protocol::DriverCommand m_driverCommand{};
};

//=============================================================================
// Template implementation
//=============================================================================

template<typename Func>
void ButtonInput::ForEachButton(Func&& func)
{
    func(m_forwardButton);
    func(m_reverseButton);
    func(m_leftButton);
    func(m_rightButton);
    func(m_turboButton);
    func(m_gravityButton);
}

} // namespace MK

#endif // MK_BUTTON_INPUT_H