/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ReceiverController.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Orquestador principal del firmware Receiver.
 * Coordina todos los medios de comunicación y el control del vehículo.
 ******************************************************************************/

#ifndef MK_RECEIVER_CONTROLLER_H
#define MK_RECEIVER_CONTROLLER_H 

//=============================================================================
// Includes
//=============================================================================

#include <cstdint>

#include "src/Communication/BluetoothTransport.h"
#include "src/Communication/ESPNowReceiver.h"

#include "src/Debug/ConsoleLogger.h"

#include <Protocol/Protocol.h>
#include <Protocol/DriverCommandSerializer.h>

#include "src/Vehicle/VehicleController.h"

namespace MK
{

class ReceiverController final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    [[nodiscard]]
    bool Begin() noexcept;

    //=========================================================================
    // Ejecución
    //=========================================================================

    void Update() noexcept;

private:

    //=========================================================================
    // Procesamiento
    //=========================================================================

    /// Procesa un paquete recibido desde cualquier transporte.
    void ProcessPacket(
        const std::uint8_t* packet,
        std::size_t length) noexcept;

private:

    //=========================================================================
    // Debug
    //=========================================================================

    ConsoleLogger m_logger;

    //=========================================================================
    // Comunicación
    //=========================================================================

    /// Transporte Bluetooth Classic.
    BluetoothTransport m_bluetooth;

    /// Transporte ESP-NOW.
    ESPNowReceiver m_receiver;

    //=========================================================================
    // Vehículo
    //=========================================================================

    VehicleController m_vehicle;

    //=========================================================================
    // Buffers
    //=========================================================================

    std::uint8_t m_packet[
        Protocol::DriverCommandSerializer::PacketSize];
};

} // namespace MK

#endif // MK_RECEIVER_CONTROLLER_H