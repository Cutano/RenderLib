#include "Base.h"
#include "SceneWindow.h"
#include "Function/Graphics/ImGui/GuiSystem.h"
#include "Utility/Event/Events.h"
#include "Utility/Event/EventBus.h"
#include "Utility/Event/EventListener.h"
#include "Utility/Preference/PreferenceManager.h"

#include <imgui.h>

namespace RL
{
    SceneWindow::SceneWindow(uint16_t index)
    : GuiWindowBase(std::wstring(L"Scene").append(std::to_wstring(index + 1)))
    {
        std::wstring sceneWindowPrefPath = L"/windowPreference/sceneWindow/showSceneWindow";
        m_Show = PreferenceManager::Get().GetSpecificPreference<bool>(sceneWindowPrefPath.append(std::to_wstring(index + 1)));
        m_CachedShow = m_Show;
        m_Index = index;

        m_Listener->SubscribeEvent<ToggleSceneWindowEvent>([this](const ToggleSceneWindowEvent& e)
        {
            if (e.Sender != this && e.Index == m_Index && e.Show != m_Show)
            {
                m_Show = e.Show;
            }

            if (m_Show == true)
            {
                OnShow();
            }
            else
            {
                OnClosed();
            }
        });
    }

    void SceneWindow::Draw()
    {
        if (m_Show)
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
            
                EventBus::Get().SpreadEvent<SceneViewportResizeEvent>({{m_Index}, {view.x, view.y}});
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
        
        if (m_Show != m_CachedShow)
        {
            EventBus::Get().SpreadEvent<ToggleSceneWindowEvent>({{m_Index}, m_Show, this});
            
            if (m_Show == true)
            {
                OnShow();
            }
            else
            {
                OnClosed();
            }
            
            m_CachedShow = m_Show;
        }
    }

    uint16_t SceneWindow::GetIndex()
    {
        return m_Index;
    }

    void SceneWindow::OnClosed()
    {
    }

    void SceneWindow::OnShow()
    {
    }
}
