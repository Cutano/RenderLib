#pragma once
#include "GuiWindowBase.h"

namespace RL
{
    class GuiToolWindowBase : public GuiWindowBase
    {
    public:
        explicit GuiToolWindowBase(const std::wstring& name);

        void Draw() override;
    };
}
