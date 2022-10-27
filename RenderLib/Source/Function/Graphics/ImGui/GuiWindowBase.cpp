#include "GuiWindowBase.h"

#include <utility>

namespace RL
{
    GuiWindowBase::GuiWindowBase(std::wstring name): GuiBase(std::move(name))
    {
    }

    bool& GuiWindowBase::ShouldShow()
    {
        return m_Show;
    }
}
