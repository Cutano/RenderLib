#include "GuiBase.h"
#include "Utility/Event/EventListener.h"

#include <utility>

namespace RL
{
    GuiBase::GuiBase(std::wstring name) : m_Name(std::move(name))
    {
        m_Listener = new EventListener();
    }

    GuiBase::~GuiBase()
    {
        delete m_Listener;
    }

    const std::wstring& GuiBase::GetName() const
    {
        return m_Name;
    }

    void GuiBase::SetName(const std::wstring& name)
    {
        m_Name = name;
    }
}
