#pragma once

#include <string>

namespace RL
{
    class GuiBase
    {
    public:
        explicit GuiBase(std::wstring name);
        
        virtual ~GuiBase() = default;
        GuiBase(const GuiBase& other) = delete;
        GuiBase(GuiBase&& other) noexcept = delete;
        GuiBase& operator=(const GuiBase& other) = delete;
        GuiBase& operator=(GuiBase&& other) noexcept = delete;

        virtual void Draw() = 0;

    protected:
        std::wstring m_Name;
    };
}
