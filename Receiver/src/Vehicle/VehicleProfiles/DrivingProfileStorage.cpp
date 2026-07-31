/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DrivingProfileStorage.cpp
 ******************************************************************************/

#include "DrivingProfileStorage.h"

#include <Preferences.h>

namespace MK::VehicleProfiles
{

//=============================================================================
// Constantes
//=============================================================================

namespace
{
constexpr const char* Namespace =
    "vehicle";

constexpr const char* Key =
    "profile";
}

//=============================================================================
// Inicialización
//=============================================================================

bool DrivingProfileStorage::Begin() noexcept
{
    return true;
}

//=============================================================================
// Load
//=============================================================================

DrivingProfileId
DrivingProfileStorage::Load() noexcept
{
    Preferences preferences;

    preferences.begin(
        Namespace,
        true);

    std::uint8_t value =
        preferences.getUChar(
            Key,
            static_cast<std::uint8_t>(
                DrivingProfileId::Normal));

    preferences.end();

    return static_cast<
        DrivingProfileId>(value);
}

//=============================================================================
// Save
//=============================================================================

void DrivingProfileStorage::Save(
    DrivingProfileId profile) noexcept
{
    Preferences preferences;

    preferences.begin(
        Namespace,
        false);

    preferences.putUChar(
        Key,
        static_cast<std::uint8_t>(
            profile));

    preferences.end();
}

}