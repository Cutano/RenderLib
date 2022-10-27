#pragma once

#include <memory>
#include <ImGuiImplDiligent.hpp>

namespace RL
{
    class Window;
    class EventListener;
    
    class ImGuiImplRenderLib final : protected Diligent::ImGuiImplDiligent
    {
    public:
        ImGuiImplRenderLib(const std::shared_ptr<Window>& window, Diligent::IRenderDevice* pDevice);
        
        ~ImGuiImplRenderLib() override;
        ImGuiImplRenderLib(const ImGuiImplRenderLib& other) = delete;
        ImGuiImplRenderLib(ImGuiImplRenderLib&& other) noexcept = delete;
        ImGuiImplRenderLib& operator=(const ImGuiImplRenderLib& other) = delete;
        ImGuiImplRenderLib& operator=(ImGuiImplRenderLib&& other) noexcept = delete;

        void NewFrame();
        
        void Render(Diligent::IDeviceContext* pCtx) override;

        void EndFrame() override;

    private:
        std::shared_ptr<EventListener> m_Listener;
        std::shared_ptr<Window> m_MainWindow;
    };
}
