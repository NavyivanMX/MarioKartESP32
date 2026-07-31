/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DrivingProfiles.cpp
 * Autor    : Narciso Ivan Cisneros Acosta
 *
 * Descripción:
 * Perfiles de conducción disponibles.
 ******************************************************************************/

#include "DrivingProfiles.h"

namespace MK::VehicleProfiles
{

const DrivingProfile Profiles[ProfileCount] =
{
    {
        "Rookie",
        100,
        80,
        180,
        0.50f,
        false,
        true
    },

    {
        "Normal",
        180,
        180,
        255,
        0.80f,
        false,
        true
    },

    {
        "Advanced",
        220,
        200,
        255,
        0.90f,
        false,
        true
    },

    {
        "Drift",
        220,
        180,
        255,
        0.35f,
        false,
        true
    }
};

}