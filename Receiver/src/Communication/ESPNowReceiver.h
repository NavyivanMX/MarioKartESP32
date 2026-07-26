/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowReceiver.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Encapsula la comunicación inalámbrica mediante ESP-NOW.
 * Se encarga de inicializar el protocolo, recibir paquetes,
 * deserializarlos y almacenar el último DriverCommand recibido.
 ******************************************************************************/

#ifndef MK_RECEIVER_ESPNOWRECEIVER_H
#define MK_RECEIVER_ESPNOWRECEIVER_H

//=============================================================================
// Includes
//=============================================================================

#include <esp_now.h>

#include <MKShared.h>

namespace MK
{

class ESPNowReceiver final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    /// Inicializa ESP-NOW y registra el callback de recepción.
    [[nodiscard]]
    bool Begin() noexcept;

    //=========================================================================
    // Comunicación
    //=========================================================================

    /// Indica si existe un comando nuevo pendiente de procesar.
    [[nodiscard]]
    bool HasNewCommand() const noexcept;

    /// Devuelve el último comando recibido.
    [[nodiscard]]
    const Protocol::DriverCommand&
    GetCommand() noexcept;

private:

    //=========================================================================
    // Callbacks
    //=========================================================================

    /// Callback invocado por ESP-NOW al recibir un paquete.
    static void OnReceive(
        const esp_now_recv_info_t* info,
        const std::uint8_t* data,
        int length);

private:

    //=========================================================================
    // Inicialización
    //=========================================================================

    [[nodiscard]]
    bool InitializeWiFi() noexcept;

    [[nodiscard]]
    bool InitializeESPNow() noexcept;

private:

    //=========================================================================
    // Estado
    //=========================================================================

    static Protocol::DriverCommand m_command;

    static bool m_hasNewCommand;
};

} // namespace MK

#endif // MK_RECEIVER_ESPNOWRECEIVER_H