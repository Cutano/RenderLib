#include "GuiBase.h"

#include <utility>

namespace RL
{
    GuiBase::GuiBase(std::wstring name) : m_Name(std::move(name))
    {
        
    }
}
