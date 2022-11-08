#include "MainMenuBar.h"
#include "Function/Scripting/ScriptingEngine.h"

#include <imgui.h>

namespace RL
{
    MainMenuBar::MainMenuBar() : GuiBase(L"MainMenuBar")
    {
        
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

                if (ImGui::MenuItem("Compile & Reload"))
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
            
            ImGui::EndMenuBar();
        }
    }

    void MainMenuBar::SetAutoRecompile(const bool autoRecompile)
    {
        m_AutoRecompile = autoRecompile;
    }

    bool& MainMenuBar::IsAutoRecompile()
    {
        return m_AutoRecompile;
    }
}
