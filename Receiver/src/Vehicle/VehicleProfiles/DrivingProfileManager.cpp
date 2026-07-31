/******************************************************************************
 * Proyecto : MarioKart ESP32 RC
 * Archivo  : DrivingProfileManager.cpp
 ******************************************************************************/

#include "DrivingProfileManager.h"

namespace MK::VehicleProfiles
{

//=============================================================================
// Ciclo de vida
//=============================================================================

bool DrivingProfileManager::Begin() noexcept
{
    if (!m_storage.Begin())
    {
        return false;
    }

    m_currentId =
        m_storage.Load();

    m_profileChanged = true;

    return true;
}

//=============================================================================
// Perfil actual
//=============================================================================

const DrivingProfile&
DrivingProfileManager::Current() const noexcept
{
    return Profiles[
        static_cast<std::size_t>(
            m_currentId)];
}

DrivingProfileId
DrivingProfileManager::CurrentId() const noexcept
{
    return m_currentId;
}

const char*
DrivingProfileManager::CurrentName() const noexcept
{
    return Current().name;
}

//=============================================================================
// Navegación
//=============================================================================

void DrivingProfileManager::Next() noexcept
{
    std::size_t index =
        static_cast<std::size_t>(
            m_currentId);

    ++index;

    if (index >= ProfileCount)
    {
        index = 0;
    }

    m_currentId =
        static_cast<DrivingProfileId>(
            index);

    m_storage.Save(
        m_currentId);

    m_profileChanged = true;
}

//-----------------------------------------------------------------------------

void DrivingProfileManager::Previous() noexcept
{
    std::size_t index =
        static_cast<std::size_t>(
            m_currentId);

    if (index == 0)
    {
        index = ProfileCount - 1;
    }
    else
    {
        --index;
    }

    m_currentId =
        static_cast<DrivingProfileId>(
            index);

    m_storage.Save(
        m_currentId);

    m_profileChanged = true;
}

//-----------------------------------------------------------------------------

void DrivingProfileManager::Set(
    DrivingProfileId profile) noexcept
{
    if (profile == m_currentId)
    {
        return;
    }

    m_currentId = profile;

    m_storage.Save(
        profile);

    m_profileChanged = true;
}

//=============================================================================
// Notificación
//=============================================================================

bool DrivingProfileManager::ProfileChanged() const noexcept
{
    return m_profileChanged;
}

void DrivingProfileManager::ClearProfileChanged() noexcept
{
    m_profileChanged = false;
}

}