/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : BluetoothInput.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Implementación de una fuente de entrada basada en Bluetooth Classic.
 * Recibe paquetes enviados desde la aplicación Android y los convierte
 * en DriverCommand.
 ******************************************************************************/

#ifndef MK_BLUETOOTH_INPUT_H
#define MK_BLUETOOTH_INPUT_H

//=============================================================================
// Includes
//=============================================================================

#include <MKShared.h>

#include "src/Input/IInputSource.h"

namespace MK
{

class BluetoothInput final :
    public IInputSource
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    /// Inicializa Bluetooth.
    void Begin() noexcept override;

    /// Procesa los datos recibidos.
    ///
    /// Devuelve true únicamente cuando llega un nuevo DriverCommand.
    [[nodiscard]]
    bool Update() noexcept override;

    //=========================================================================
    // Acceso
    //=========================================================================

    [[nodiscard]]
    const Protocol::DriverCommand&
    GetDriverCommand() const noexcept override;

    //=========================================================================
    // Estado
    //=========================================================================

    /// Indica si existe un dispositivo conectado.
    [[nodiscard]]
    bool IsConnected() const noexcept;

private:

    //=========================================================================
    // Recepción
    //=========================================================================

    /// Procesa un paquete recibido desde Android.
    [[nodiscard]]
    bool ReceivePacket() noexcept;

private:

    //=========================================================================
    // Estado
    //=========================================================================

    Protocol::DriverCommand m_driverCommand{};

    bool m_connected = false;
};

} // namespace MK

#endif // MK_BLUETOOTH_INPUT_H