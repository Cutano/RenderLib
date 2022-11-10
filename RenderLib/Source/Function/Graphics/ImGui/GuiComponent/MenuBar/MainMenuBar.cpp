#include "MainMenuBar.h"
#include "Function/Scripting/ScriptingEngine.h"
#include "Function/Graphics/ImGui/GuiSystem.h"
#include "Utility/Event/EventListener.h"
#include "Utility/Event/Events.h"
#include "Utility/Event/EventBus.h"
#include "Utility/Preference/PreferenceManager.h"
#include "Utility/Preference/WindowPreference.h"

#include <imgui.h>

namespace RL
{
    MainMenuBar::MainMenuBar() : GuiBase(L"MainMenuBar")
    {
        m_Listener->SubscribeEvent<SourceFileChangedEvent>([this](const SourceFileChangedEvent&)
        {
            if (IsAutoRecompile())
            {
                ScriptingEngine::Get().BuildAndLoadAssemblies();
            }
        });

        m_Listener->SubscribeEvent<ToggleSceneWindowEvent>([this](const ToggleSceneWindowEvent& e)
        {
            if (e.Sender != this)
            {
                ToggleSceneWindow(e.Index, e.Show, false);
            }
        });

        LoadPreferences();
    }

    void MainMenuBar::Draw()
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Scripting"))
            {
                if (ImGui::MenuItem("Compile Script"))
                {
                    ScriptingEngine::Get().BuildAssemblies();
                }

                if (ImGui::MenuItem("Reload Assembly"))
                {
                    ScriptingEngine::Get().LoadAssemblies();
                }

                if (ImGui::MenuItem("Compile & Reload", "Ctrl+R"))
                {
                    ScriptingEngine::Get().BuildAndLoadAssemblies();
                }

                if (ImGui::MenuItem("Unload Assembly"))
                {
                    ScriptingEngine::Get().UnloadAssemblies();
                }

                if (ImGui::MenuItem("Auto Recompile", nullptr, m_AutoRecompile, true))
                {
                    SetAutoRecompile(!m_AutoRecompile);
                }
                
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Window"))
            {
                if (ImGui::BeginMenu("SceneWindow"))
                {
                    if (ImGui::MenuItem("Scene Window 1", nullptr, m_ShowSceneWindow[0]))
                    {
                        ToggleSceneWindow(0, !m_ShowSceneWindow[0], true);
                    }
                    if (ImGui::MenuItem("Scene Window 2", nullptr, m_ShowSceneWindow[1]))
                    {
                        ToggleSceneWindow(1, !m_ShowSceneWindow[1], true);
                    }
                    if (ImGui::MenuItem("Scene Window 3", nullptr, m_ShowSceneWindow[2]))
                    {
                        ToggleSceneWindow(2, !m_ShowSceneWindow[2], true);
                    }
                    if (ImGui::MenuItem("Scene Window 4", nullptr, m_ShowSceneWindow[3]))
                    {
                        ToggleSceneWindow(3, !m_ShowSceneWindow[3], true);
                    }
                
                    ImGui::EndMenu();
                }
                
                ImGui::EndMenu();
            }
            
            ImGui::EndMenuBar();
        }
    }

    bool& MainMenuBar::IsAutoRecompile()
    {
        return m_AutoRecompile;
    }

    void MainMenuBar::SetAutoRecompile(const bool autoRecompile)
    {
        m_AutoRecompile = autoRecompile;
        PreferenceManager::Get().SetSpecificPreference<bool>(L"/scriptingPreference/autoCompile", autoRecompile, false);
    }

    void MainMenuBar::ToggleSceneWindow(const int index, const bool show, const bool spread)
    {
        if (m_ShowSceneWindow[index] != show)
        {
            m_ShowSceneWindow[index] = show;
            std::wstring path = L"/windowPreference/sceneWindow/showSceneWindow";
            PreferenceManager::Get().SetSpecificPreference<bool>(path.append(std::to_wstring(index + 1)), show, false);
        }

        if (spread)
        {
            EventBus::Get().SpreadEvent<ToggleSceneWindowEvent>({{index}, show, this});
        }
    }

    void MainMenuBar::LoadPreferences()
    {
        m_AutoRecompile = PreferenceManager::Get().GetSpecificPreference<bool>(L"/scriptingPreference/autoCompile");

        const std::wstring sceneWindowPrefPath = L"/windowPreference/sceneWindow";
        const auto sceneWindowPref = PreferenceManager::Get().GetSpecificPreference<SceneWindowPreference>(sceneWindowPrefPath);
        m_ShowSceneWindow[0] = sceneWindowPref.ShowSceneWindow1;
        m_ShowSceneWindow[1] = sceneWindowPref.ShowSceneWindow2;
        m_ShowSceneWindow[2] = sceneWindowPref.ShowSceneWindow3;
        m_ShowSceneWindow[3] = sceneWindowPref.ShowSceneWindow4;
    }
}
