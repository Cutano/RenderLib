#include "GuiSystem.h"

#include "GuiBase.h"
#include "GuiComponent/Window/DemoWindow.h"
#include "GuiComponent/Window/SceneWindow.h"
#include "GuiComponent/MenuBar/MainMenuBar.h"
#include "Platform/Workspace/Workspace.h"
#include "Utility/Log.h"

#include <imgui.h>
#include <filesystem>

namespace RL
{
    static std::string s_ImGuiSettingPath;
    
    void GuiSystem::Init()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        m_ImGuiCtx = ImGui::CreateContext();
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(m_ImGuiCtx));
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

        const std::filesystem::path workDir = Workspace::Get().GetWorkspaceDir();
        s_ImGuiSettingPath = (workDir / "ImGui.ini").string();
        io.IniFilename = s_ImGuiSettingPath.c_str();

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        m_GuiRegistry.emplace_back(std::make_shared<DemoWindow>());
        m_GuiRegistry.emplace_back(std::make_shared<MainMenuBar>());
        m_GuiRegistry.emplace_back(std::make_shared<SceneWindow>(0));
        m_GuiRegistry.emplace_back(std::make_shared<SceneWindow>(1));
        m_GuiRegistry.emplace_back(std::make_shared<SceneWindow>(2));
        m_GuiRegistry.emplace_back(std::make_shared<SceneWindow>(3));
    }

    void GuiSystem::Update()
    {
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(m_ImGuiCtx));
        
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        ImGui::Begin("DockSpaceWindow", nullptr, windowFlags);
        ImGui::PopStyleVar(3);
        const ImGuiID dockSpaceId = ImGui::GetID("DockSpace");
        ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f));

        for (const auto& guiWindow : m_GuiRegistry)
        {
            guiWindow->Draw();
        }

        ImGui::End();
    }

    std::shared_ptr<GuiBase> GuiSystem::GetUIComponentByName(const std::wstring& name) const
    {
        for (const auto& ui : m_GuiRegistry)
        {
            if (ui->GetName() == name)
            {
                return ui;
            }
        }

        return nullptr;
    }
}
