#include "DemoWindow.h"
#include "Utility/Event/Events.h"
#include "Utility/Event/EventBus.h"
#include "Utility/Event/EventListener.h"
#include "Utility/Preference/PreferenceManager.h"

#include <imgui.h>

namespace RL
{
    DemoWindow::DemoWindow() : GuiWindowBase(L"ImGuiDemoWindow")
    {
        const std::wstring demoWindowPrefPath = L"/windowPreference/showImGuiDemoWindow";
        m_Show = PreferenceManager::Get().GetSpecificPreference<bool>(demoWindowPrefPath);
        m_CachedShow = m_Show;

        m_Listener->SubscribeEvent<ToggleToolWindowEvent>([this](const ToggleToolWindowEvent& e)
        {
            if (e.Sender != this && e.Name == m_Name && e.Show != m_Show)
            {
                m_Show = e.Show;
            }
        });
    }

    void DemoWindow::Draw()
    {
        if (m_Show)
        {
            ImGui::ShowDemoWindow(&m_Show);
        }

        if (m_Show != m_CachedShow)
        {
            EventBus::Get().SpreadEvent<ToggleToolWindowEvent>({{{this}, m_Name}, m_Show});
            m_CachedShow = m_Show;
        }
    }
}
