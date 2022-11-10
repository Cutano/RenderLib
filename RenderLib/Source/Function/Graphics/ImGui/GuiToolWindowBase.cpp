#include "GuiToolWindowBase.h"

#include "Utility/Event/EventBus.h"
#include "Utility/Event/EventListener.h"
#include "Utility/Event/Events.h"
#include "Utility/Preference/PreferenceManager.h"

namespace RL
{
    GuiToolWindowBase::GuiToolWindowBase(const std::wstring& name) : GuiWindowBase(name)
    {
        const std::wstring toolWindowPrefPath = std::wstring(L"/windowPreference/").append(L"show").append(name);
        m_Show = PreferenceManager::Get().GetSpecificPreference<bool>(toolWindowPrefPath);
        m_CachedShow = m_Show;

        m_Listener->SubscribeEvent<ToggleToolWindowEvent>([this](const ToggleToolWindowEvent& e)
        {
            if (e.Sender != this && e.Name == m_Name && e.Show != m_Show)
            {
                m_Show = e.Show;
            }
        });
    }

    void GuiToolWindowBase::Draw()
    {
        if (m_Show != m_CachedShow)
        {
            EventBus::Get().SpreadEvent<ToggleToolWindowEvent>({{{this}, m_Name}, m_Show});
            m_CachedShow = m_Show;
        }
    }
}
