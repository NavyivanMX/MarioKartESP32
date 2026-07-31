/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DrivingProfile.h
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Define un perfil de conducción del vehículo.
 * Cada perfil representa el comportamiento dinámico del kart.
 ******************************************************************************/

#ifndef MK_RECEIVER_DRIVINGPROFILE_H
#define MK_RECEIVER_DRIVINGPROFILE_H

//=============================================================================
// Includes
//=============================================================================

#include <cstdint>

namespace MK::VehicleProfiles
{

//=============================================================================
// DrivingProfile
//=============================================================================

struct DrivingProfile
{
    //---------------------------------------------------------------------
    // Identificación
    //---------------------------------------------------------------------

    const char* name;

    //---------------------------------------------------------------------
    // Velocidades
    //---------------------------------------------------------------------

    std::uint8_t forwardSpeed;

    std::uint8_t reverseSpeed;

    std::uint8_t turboSpeed;

    //---------------------------------------------------------------------
    // Dirección
    //---------------------------------------------------------------------

    float steeringFactor;

    //---------------------------------------------------------------------
    // Capacidades
    //---------------------------------------------------------------------

    bool gravityEnabled;

    bool turboEnabled;
};

} // namespace MK::VehicleProfiles

#endif // MK_RECEIVER_DRIVINGPROFILE_H