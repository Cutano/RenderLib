#include "MainMenuBar.h"
#include "Function/Scripting/ScriptingEngine.h"
#include "Function/Graphics/ImGui/GuiSystem.h"
#include "Utility/Event/EventListener.h"
#include "Utility/Event/Events.h"
#include "Utility/Preference/PreferenceManager.h"

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

        m_AutoRecompile = PreferenceManager::Get().GetSpecificPreference<bool>(L"/scriptingPreference/autoCompile");
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
                if (ImGui::MenuItem("Add Scene Window"))
                {
                    GuiSystem::Get().AddSceneWindowNextFrame();
                }
                
                ImGui::EndMenu();
            }
            
            ImGui::EndMenuBar();
        }
    }

    void MainMenuBar::SetAutoRecompile(const bool autoRecompile)
    {
        m_AutoRecompile = autoRecompile;
        PreferenceManager::Get().SetSpecificPreference<bool>(L"/scriptingPreference/autoCompile", autoRecompile, false);
    }

    bool& MainMenuBar::IsAutoRecompile()
    {
        return m_AutoRecompile;
    }
}
