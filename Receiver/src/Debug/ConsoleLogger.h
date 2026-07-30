/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : ConsoleLogger.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Logger de consola del Receiver.
 * Centraliza todos los mensajes de depuración y diagnóstico del sistema.
 ******************************************************************************/

#ifndef MK_RECEIVER_CONSOLELOGGER_H
#define MK_RECEIVER_CONSOLELOGGER_H

#include <MKShared.h>

namespace MK
{

class ConsoleLogger final
{
public:

    //=========================================================================
    // Ciclo de vida
    //=========================================================================

    /// Inicializa la consola serie.
    void Begin() noexcept;

    //=========================================================================
    // Sistema
    //=========================================================================

    /// Muestra la información de arranque.
    void LogBoot() const noexcept;

    /// Indica que el sistema está listo.
    void LogReady() const noexcept;

    /// Muestra un mensaje de error.
    void LogError(
        const char* message) const noexcept;

    //=========================================================================
    // Comunicación
    //=========================================================================

    /// Muestra la dirección MAC WiFi del ESP32.
    void LogWiFiMac() const noexcept;

    /// Muestra el nombre Bluetooth del dispositivo.
    void LogBluetooth(
        const char* deviceName) const noexcept;

    /// Indica que un cliente Bluetooth se conectó.
    void LogBluetoothConnected() const noexcept;

    //=========================================================================
    // DriverCommand
    //=========================================================================

    /// Muestra el DriverCommand recibido.
    void Log(
        const Protocol::DriverCommand& command) const noexcept;

private:

    //=========================================================================
    // Conversión de enums
    //=========================================================================

    [[nodiscard]]
    static const char* ToString(
        Types::Vehicle::Direction direction) noexcept;

    [[nodiscard]]
    static const char* ToString(
        Types::Vehicle::Steering steering) noexcept;

    [[nodiscard]]
    static const char* ToString(
        Types::Vehicle::Turbo turbo) noexcept;

    [[nodiscard]]
    static const char* ToString(
        Types::Vehicle::DriveMode mode) noexcept;
};

} // namespace MK

#endif // MK_RECEIVER_CONSOLELOGGER_H