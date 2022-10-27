#pragma once
#include "GuiBase.h"

namespace RL
{
    class GuiWindowBase : public GuiBase
    {
    public:
        explicit GuiWindowBase(std::wstring name);
        
        ~GuiWindowBase() override = default;
        GuiWindowBase(const GuiWindowBase& other) = delete;
        GuiWindowBase(GuiWindowBase&& other) noexcept = delete;
        GuiWindowBase& operator=(const GuiWindowBase& other) = delete;
        GuiWindowBase& operator=(GuiWindowBase&& other) noexcept = delete;

    protected:
        
    };
    
}
