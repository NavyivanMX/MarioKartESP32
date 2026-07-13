/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : InputManager.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Administra las entradas físicas del transmisor y construye el comando
 * que será enviado al vehículo.
 ******************************************************************************/

#ifndef MK_INPUT_MANAGER_H
#define MK_INPUT_MANAGER_H

// #include "src/Hardware/Button.h"
// #include "src/Hardware/Pins.h"

// #include "src/Shared/Protocol.h"

#include <MKShared.h>

namespace MK
{

class InputManager final
{
public:

    InputManager() = default;

    ~InputManager() = default;

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    void Begin();

    void Update();

    //=========================================================================
    // Acceso
    //=========================================================================

    [[nodiscard]]
    const Protocol::DriverCommand& GetDriverCommand() const noexcept;

private:

    //=========================================================================
    // Actualización
    //=========================================================================

    void ReadButtons();

    void BuildDriverCommand();

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
void InputManager::ForEachButton(Func&& func)
{
    func(m_forwardButton);
    func(m_reverseButton);
    func(m_leftButton);
    func(m_rightButton);
    func(m_turboButton);
    func(m_gravityButton);
}

} // namespace MK

#endif // MK_INPUT_MANAGER_H