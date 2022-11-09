#pragma once

#include <string>

namespace RL
{
    class EventListener;
    
    class GuiBase
    {
    public:
        explicit GuiBase(std::wstring name);
        
        virtual ~GuiBase();
        GuiBase(const GuiBase& other) = delete;
        GuiBase(GuiBase&& other) noexcept = delete;
        GuiBase& operator=(const GuiBase& other) = delete;
        GuiBase& operator=(GuiBase&& other) noexcept = delete;

        virtual void Draw() = 0;
        
        [[nodiscard]] virtual const std::wstring& GetName() const;
        virtual void SetName(const std::wstring& name);
        
    protected:
        std::wstring m_Name;

        EventListener* m_Listener;
    };
}
