#include "Base.h"
#include "SceneWindow.h"
#include "Utility/Event/Events.h"
#include "Utility/Event/EventBus.h"

#include <imgui.h>

namespace RL
{
    SceneWindow::SceneWindow() : GuiWindowBase(L"DefaultSceneWindow")
    {
        m_Show = true;
    }

    SceneWindow::SceneWindow(const std::wstring& name) : GuiWindowBase(name)
    {
        m_Show = true;
    }

    void SceneWindow::Draw()
    {
        constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin(ConvertString(m_Name).c_str(), &m_Show, windowFlags);
        ImGui::PopStyleVar();

        m_IsSceneWindowHovered = ImGui::IsWindowHovered();

        const ImVec2 view = ImGui::GetContentRegionAvail();
        if (view.x != m_SceneWindowWidth || view.y != m_SceneWindowHeight)
        {
            if (view.x <= 0 || view.y <= 0)
            {
                // The window is too small or collapsed.
                ImGui::End();
                return;
            }

            m_SceneWindowWidth = view.x;
            m_SceneWindowHeight = view.y;
            
            EventBus::Get().SpreadEvent<SceneViewportResizeEvent>({{m_Name}, {view.x, view.y}});
        }

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();
        vMin.x += ImGui::GetWindowPos().x;
        vMin.y += ImGui::GetWindowPos().y;
        vMax.x += ImGui::GetWindowPos().x;
        vMax.y += ImGui::GetWindowPos().y;
        // drawList->AddImage(textureId, vMin, vMax);

        m_SceneWindowPosX = vMin.x;
        m_SceneWindowPosY = vMin.y;

        ImGui::End();
    }
}
