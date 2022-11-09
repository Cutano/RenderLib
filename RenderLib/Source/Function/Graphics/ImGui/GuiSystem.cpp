#include "GuiSystem.h"
#include "GuiBase.h"
#include "GuiComponent/Window/DemoWindow.h"
#include "GuiComponent/Window/SceneWindow.h"
#include "GuiComponent/MenuBar/MainMenuBar.h"
#include "Utility/Log.h"

#include <imgui.h>

namespace RL
{
    static bool g_NewSceneWindowNextFrame {false};
    static int32_t g_CloseSceneWindowNextFrame {-1};
    
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

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        m_GuiRegistry.emplace_back(std::make_shared<DemoWindow>());
        m_GuiRegistry.emplace_back(std::make_shared<MainMenuBar>());
        NewSceneWindow();
    }

    void GuiSystem::Update()
    {
        if (g_NewSceneWindowNextFrame)
        {
            if (!NewSceneWindow())
            {
                Log::Logger()->warn("Max scene window count is {}", m_AvailableSceneWindow.size());
            }
        }

        if (g_CloseSceneWindowNextFrame != -1)
        {
            RemoveSceneWindow(static_cast<uint16_t>(g_CloseSceneWindowNextFrame));
            g_CloseSceneWindowNextFrame = -1;
        }
        
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

    void GuiSystem::AddSceneWindowNextFrame()
    {
        g_NewSceneWindowNextFrame = true;
    }

    void GuiSystem::RemoveSceneWindowNextFrame(uint16_t number)
    {
        g_CloseSceneWindowNextFrame = number;
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

    bool GuiSystem::NewSceneWindow()
    {
        g_NewSceneWindowNextFrame = false;
        
        for (int i = 0; i < MAX_SCENE_WINDOW_COUNT; ++i)
        {
            if (m_AvailableSceneWindow.at(i) == false)
            {
                m_GuiRegistry.emplace_back(std::make_shared<SceneWindow>(i));
                m_AvailableSceneWindow[i] = true;

                return true;
            }
        }

        return false;
    }

    void GuiSystem::RemoveSceneWindow(uint16_t number)
    {
        for (auto it = m_GuiRegistry.begin(); it != m_GuiRegistry.end(); ++it)
        {
            if (const auto window = dynamic_cast<SceneWindow*>((*it).get()))
            {
                if (window->GetID() == number)
                {
                    m_GuiRegistry.erase(it);
                    m_AvailableSceneWindow[number] = false;
                    return;
                }
            }
        }
    }
}
