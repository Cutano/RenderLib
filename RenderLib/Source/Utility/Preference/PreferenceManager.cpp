#include "PreferenceManager.h"
#include "ScriptingPreference.h"
#include "WindowPreference.h"
#include "Platform/Workspace/Workspace.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <thread>

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

        Load(false);
        InitPreferences();
    }

    void PreferenceManager::Shutdown()
    {

    }

    void PreferenceManager::Save(bool async)
    {
        const auto save = [this]
        {
            const auto global = m_GlobalPreference.dump(4);
            const auto project = m_ProjectPreference.dump(4);

            std::ofstream globalFile;
            globalFile.open(m_GlobalPreferencePath, std::ios::out | std::ios::trunc);
            globalFile << global << std::endl;
            globalFile.close();

            std::ofstream projectFile;
            projectFile.open(m_ProjectPreferencePath, std::ios::out | std::ios::trunc);
            projectFile << project << std::endl;
            projectFile.close();

            Log::Logger()->info("Preferences saved.");
        };
        
        if (async)
        {
            std::thread{save}.detach();
        }
        else
        {
            save();
        }
    }

    void PreferenceManager::Load(bool async)
    {
        const auto load = [this]{
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
        };

        if (async)
        {
            std::thread{load}.detach();
        }
        else
        {
            load();
        }
    }

    bool PreferenceManager::HasPreference(const std::wstring& name) const
    {
        const auto key = ConvertString(name);
        return m_ProjectPreference.contains(key) || m_GlobalPreference.contains(key);
    }

    void PreferenceManager::SetPreference(const std::wstring& name, const IPreference& preference, const bool isGlobal, bool async)
    {
        if (isGlobal)
        {
            m_GlobalPreference[ConvertString(name)] = preference;
        }
        else
        {
            m_ProjectPreference[ConvertString(name)] = preference;
        }

        Save(async);
    }

    void PreferenceManager::SetSimplePreference(const std::wstring& name, const std::wstring& preference, bool isGlobal, bool async)
    {
        if (isGlobal)
        {
            m_GlobalPreference[ConvertString(name)] = preference;
        }
        else
        {
            m_ProjectPreference[ConvertString(name)] = preference;
        }

        Save(async);
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

    void PreferenceManager::InitPreferences()
    {
        const ScriptingPreference scriptingPreference {};
        if (const auto scriptingPreferenceName = L"scriptingPreference"; !HasPreference(scriptingPreferenceName))
        {
            SetPreference(scriptingPreferenceName, scriptingPreference, false, false);
        }

        const WindowPreference windowPreference {};
        if (const auto windowPreferenceName = L"windowPreference"; !HasPreference(windowPreferenceName))
        {
            SetPreference(windowPreferenceName, windowPreference, false, false);
        }
    }

}
