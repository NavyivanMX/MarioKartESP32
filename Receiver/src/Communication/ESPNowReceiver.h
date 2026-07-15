/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ESPNowReceiver.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Encapsula la comunicación inalámbrica mediante ESP-NOW.
 * Se encarga de inicializar el protocolo y almacenar el último
 * comando recibido para su posterior procesamiento.
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
        const uint8_t* data,
        int len);
        
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

    /// Último comando recibido.
    static Protocol::DriverCommand m_command;

    /// Indica si existe un comando nuevo pendiente de procesar.
    static bool m_hasNewCommand;
};

} // namespace MK

#endif // MK_RECEIVER_ESPNOWRECEIVER_H