#include "PreferenceManager.h"
#include "ScriptingPreference.h"
#include "Platform/Workspace/Workspace.h"

#include <nlohmann/json.hpp>
#include <fstream>

namespace RL
{
    void PreferenceManager::Init()
    {
        const auto globalDir = std::filesystem::path {Workspace::Get().GetApplicationPath()}.parent_path();
        const auto projDir = std::filesystem::path {Workspace::Get().GetWorkspaceDir()};

        // Create "preferences" directory if doesn't exist
        if (!exists(globalDir / "Preferences"))
        {
            create_directories(globalDir / "Preferences");
        }

        m_GlobalPreferencePath = (globalDir / "Preferences" / "Preference.json").string();
        m_ProjectPreferencePath = (projDir / "Preference.json").string();

        Load();

        const ScriptingPreference scriptingPreference {};
        if (const auto scriptingPreferenceName = L"scriptingPreference"; !HasPreference(scriptingPreferenceName))
        {
            SetPreference(scriptingPreferenceName, scriptingPreference);
        }
    }

    void PreferenceManager::Shutdown()
    {
        Save();
    }

    void PreferenceManager::Save()
    {
        std::scoped_lock lk {m_SaveMutex, m_LoadMutex};
            
        const auto global =  m_GlobalPreference.dump(4);
        const auto project =  m_ProjectPreference.dump(4);

        std::ofstream globalFile;
        globalFile.open(m_GlobalPreferencePath, std::ios::out | std::ios::trunc);
        globalFile << global << std::endl;
        globalFile.close();

        std::ofstream projectFile;
        projectFile.open(m_ProjectPreferencePath, std::ios::out | std::ios::trunc);
        projectFile << project << std::endl;
        projectFile.close();

        Log::Logger()->info("Preferences saved.");
    }

    void PreferenceManager::Load()
    {
        std::scoped_lock lk {m_SaveMutex, m_LoadMutex};

        if (std::filesystem::exists(m_GlobalPreferencePath))
        {
            std::ifstream globalFile(m_GlobalPreferencePath);
            globalFile >> m_GlobalPreference;
            globalFile.close();

            Log::Logger()->info("Global preference loaded, path: {}", m_GlobalPreferencePath);
        }

        if (std::filesystem::exists(m_ProjectPreferencePath))
        {
            std::ifstream projectFile(m_ProjectPreferencePath);
            projectFile >> m_ProjectPreference;
            projectFile.close();

            Log::Logger()->info("Project preference loaded, path: {}", m_ProjectPreferencePath);
        }
    }

    bool PreferenceManager::HasPreference(const std::wstring& name) const
    {
        const auto key = ConvertString(name);
        return m_ProjectPreference.contains(key) || m_GlobalPreference.contains(key);
    }

    void PreferenceManager::SetPreference(const std::wstring& name, const IPreference& preference, const bool isGlobal)
    {
        if (isGlobal)
        {
            m_GlobalPreference[ConvertString(name)] = preference;
        }
        else
        {
            m_ProjectPreference[ConvertString(name)] = preference;
        }

        Save();
    }

    void PreferenceManager::SetSimplePreference(const std::wstring& name, const std::wstring& preference, bool isGlobal)
    {
        if (isGlobal)
        {
            m_GlobalPreference[ConvertString(name)] = preference;
        }
        else
        {
            m_ProjectPreference[ConvertString(name)] = preference;
        }

        Save();
    }

    std::wstring PreferenceManager::GetSimplePreference(const std::wstring& name)
    {
        if (const auto key = ConvertString(name); m_ProjectPreference.contains(key))
        {
            const auto j = m_ProjectPreference.at(key);
            return j.get<std::wstring>();
        }
        else
        {
            const auto j = m_GlobalPreference.at(key);
            return j.get<std::wstring>();
        }
    }
}
