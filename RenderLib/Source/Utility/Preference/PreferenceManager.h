#pragma once

#include "Base.h"
#include "IPreference.h"

#include <mutex>
#include <string>

namespace RL
{
    class PreferenceManager
    {
    public:
        static PreferenceManager& Get()
        {
            static PreferenceManager s_Instance;
            return s_Instance;
        }

        ~PreferenceManager() = default;
        PreferenceManager(const PreferenceManager& other) = delete;
        PreferenceManager(PreferenceManager&& other) noexcept = delete;
        PreferenceManager& operator=(const PreferenceManager& other) = delete;
        PreferenceManager& operator=(PreferenceManager&& other) noexcept = delete;

        void Init();
        void Shutdown();

        void Save();
        void Load();
        [[nodiscard]] bool HasPreference(const std::wstring& name) const;
        [[nodiscard]] std::wstring GetSimplePreference(const std::wstring& name);
        void SetSimplePreference(const std::wstring& name, const std::wstring& preference, bool isGlobal = false);
        void SetPreference(const std::wstring& name, const IPreference& preference, bool isGlobal = false);

        template <typename T> T GetSpecificPreference(const std::wstring& path)
        {
            if (const nlohmann::json::json_pointer pointer {ConvertString(path)}; m_ProjectPreference.contains(pointer))
            {
                const auto j = m_ProjectPreference.at(pointer);
                return j.get<T>();
            }
            else
            {
                const auto j = m_GlobalPreference.at(pointer);
                return j.get<T>();
            }
        }
        
        template <typename T> void SetSpecificPreference(const std::wstring& path, T preference, bool isGlobal = false)
        {
            const nlohmann::json::json_pointer pointer {ConvertString(path)};

            if (isGlobal)
            {
                m_GlobalPreference[pointer] = preference;
            }
            else
            {
                m_ProjectPreference[pointer] = preference;
            }

            Save();
        }
        
        template <typename T> T GetPreference(const std::wstring& name)
        {
            if (const auto key = ConvertString(name); m_ProjectPreference.contains(key))
            {
                const auto j = m_ProjectPreference.at(key);
                return j.get<T>();
            }
            else
            {
                const auto j = m_GlobalPreference.at(key);
                return j.get<T>();
            }
        }
        

    private:
        PreferenceManager() = default;

        nlohmann::json m_GlobalPreference;
        nlohmann::json m_ProjectPreference;

        std::mutex m_SaveMutex;
        std::mutex m_LoadMutex;

        std::string m_GlobalPreferencePath;
        std::string m_ProjectPreferencePath;
    };
}
