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

        m_Listener->SubscribeEvent<ToggleToolWindowEvent>([this](const ToggleToolWindowEvent& e)
        {
            if (e.Sender != this)
            {
                if (e.Name == L"ImGuiDemoWindow")
                {
                    ToggleImGuiDemoWindow(e.Show, false);
                }
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
                if (ImGui::BeginMenu("Scene Window"))
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

                if (ImGui::MenuItem("ImGui Demo Window", nullptr, m_ShowImGuiDemoWindow))
                {
                    ToggleImGuiDemoWindow(!m_ShowImGuiDemoWindow, true);
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
            EventBus::Get().SpreadEvent<ToggleSceneWindowEvent>({{{this}, index}, show});
        }
    }

    void MainMenuBar::ToggleImGuiDemoWindow(bool show, bool spread)
    {
        if (m_ShowImGuiDemoWindow != show)
        {
            m_ShowImGuiDemoWindow = show;
            const std::wstring path = L"/windowPreference/showImGuiDemoWindow";
            PreferenceManager::Get().SetSpecificPreference<bool>(path, show, false);
        }

        if (spread)
        {
            EventBus::Get().SpreadEvent<ToggleToolWindowEvent>({{{this}, L"ImGuiDemoWindow"}, show});
        }
    }

    void MainMenuBar::LoadPreferences()
    {
        m_AutoRecompile = PreferenceManager::Get().GetSpecificPreference<bool>(L"/scriptingPreference/autoCompile");

        const std::wstring windowPrefPath = L"/windowPreference";
        const auto windowPref = PreferenceManager::Get().GetSpecificPreference<WindowPreference>(windowPrefPath);
        m_ShowSceneWindow[0]  = windowPref.SceneWindowPref.ShowSceneWindow1;
        m_ShowSceneWindow[1]  = windowPref.SceneWindowPref.ShowSceneWindow2;
        m_ShowSceneWindow[2]  = windowPref.SceneWindowPref.ShowSceneWindow3;
        m_ShowSceneWindow[3]  = windowPref.SceneWindowPref.ShowSceneWindow4;
        m_ShowImGuiDemoWindow = windowPref.ShowImGuiDemoWindow;
    }
}
