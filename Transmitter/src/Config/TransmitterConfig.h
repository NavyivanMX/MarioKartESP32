/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : TransmitterConfig.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Configuración específica del Transmitter.
 *
 * Los receptores conocidos se mantienen en una lista estática y se recorren
 * en el orden en que aparecen.
 *
 * Si ninguno de los receptores conocidos responde, ESP-NOW intentará utilizar
 * el receptor personalizado almacenado en NVS.
 ******************************************************************************/

#ifndef MK_TRANSMITTER_CONFIG_H
#define MK_TRANSMITTER_CONFIG_H

#include <cstddef>
#include <cstdint>

#include <MKShared.h>

namespace MK::TransmitterConfig
{

//=============================================================================
// Configuración general
//=============================================================================

inline constexpr std::uint32_t TransmitPeriodMs = 50;

inline constexpr bool InputTestMode = false;


//=============================================================================
// Receptores conocidos
//
// IMPORTANTE:
// El orden de esta lista determina el orden de búsqueda.
//
// Para agregar otro receptor solamente agrega otro MAC:
//
// {
//     MAC1,
//     MAC2,
//     MAC3,
//     ...
// }
//=============================================================================

inline constexpr Types::MacAddress KnownReceiverMacs[] =
{
    // Receiver - Kart
    {
        0x28, 0x05, 0xA5, 0x0B, 0x42, 0xF8
    },

    // Receiver - Laboratorio
    {
        0xCC, 0xDB, 0xA7, 0x3E, 0xD7, 0x74
    }

    // Ejemplo para agregar otro:
    //
    // {
    //     0x11, 0x22, 0x33, 0x44, 0x55, 0x66
    // }
};


//=============================================================================
// Cantidad de receptores conocidos
//=============================================================================

inline constexpr std::size_t KnownReceiverMacCount =
    sizeof(KnownReceiverMacs) / sizeof(KnownReceiverMacs[0]);

}

#endif