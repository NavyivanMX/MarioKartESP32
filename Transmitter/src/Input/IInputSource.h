/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : IInputSource.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Interfaz para cualquier fuente de entrada del transmisor.
 * Una implementación puede obtener comandos desde botones físicos,
 * Bluetooth, USB, etc.
 ******************************************************************************/

#ifndef MK_I_INPUT_SOURCE_H
#define MK_I_INPUT_SOURCE_H

#include <MKShared.h>

namespace MK
{

class IInputSource
{
public:

    virtual ~IInputSource() = default;

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    /// Inicializa la fuente de entrada.
    virtual void Begin() noexcept = 0;

    /// Actualiza el estado de la entrada.
    ///
    /// Devuelve true únicamente cuando el DriverCommand cambia.
    [[nodiscard]]
    virtual bool Update() noexcept = 0;

    //=========================================================================
    // Acceso
    //=========================================================================

    /// Obtiene el último DriverCommand generado.
    [[nodiscard]]
    virtual const Protocol::DriverCommand&
    GetDriverCommand() const noexcept = 0;
};

} // namespace MK

#endif // MK_I_INPUT_SOURCE_H