#include "ImGuiImplRenderLib.h"
#include "Utility/Event/EventListener.h"
#include "Utility/Event/Events.h"
#include "Platform/Window/Window.h"

#include <backends/imgui_impl_glfw.h>
#include <SwapChain.h>

namespace RL
{
    ImGuiImplRenderLib::ImGuiImplRenderLib(const std::shared_ptr<Window>& window, Diligent::IRenderDevice* pDevice)
    : ImGuiImplDiligent(
        pDevice,
        window->GetSwapChain()->GetDesc().ColorBufferFormat,
        window->GetSwapChain()->GetDesc().DepthBufferFormat)
    , m_MainWindow(window)
    {
        m_Listener = std::make_shared<EventListener>();
        
        ImGui_ImplGlfw_InitForOther(window->GetGlfWWindow(), false);

        m_Listener->SubscribeEvent<AppWindowFocusEvent>([](const AppWindowFocusEvent e)
        {
            ImGui_ImplGlfw_WindowFocusCallback(e.GlfWWindow, e.Focused);
        });

        m_Listener->SubscribeEvent<MouseEnteredEvent>([](const MouseEnteredEvent e)
        {
            ImGui_ImplGlfw_CursorEnterCallback(e.GlfWWindow, e.Entered);
        });

        m_Listener->SubscribeEvent<MouseMovedEvent>([](const MouseMovedEvent e)
        {
            ImGui_ImplGlfw_CursorPosCallback(e.GlfWWindow, e.X, e.Y);
        });
        
        m_Listener->SubscribeEvent<MouseButtonPressedEvent>([](const MouseButtonPressedEvent e)
        {
            ImGui_ImplGlfw_MouseButtonCallback(e.GlfWWindow, static_cast<int>(e.Button), e.Action, e.Mods);
        });

        m_Listener->SubscribeEvent<MouseButtonReleasedEvent>([](const MouseButtonReleasedEvent e)
        {
            ImGui_ImplGlfw_MouseButtonCallback(e.GlfWWindow, static_cast<int>(e.Button), e.Action, e.Mods);
        });

        m_Listener->SubscribeEvent<MouseScrolledEvent>([](const MouseScrolledEvent e)
        {
            ImGui_ImplGlfw_ScrollCallback(e.GlfWWindow, e.X, e.Y);
        });
        
        m_Listener->SubscribeEvent<KeyPressedEvent>([](const KeyPressedEvent e)
        {
            ImGui_ImplGlfw_KeyCallback(e.GlfWWindow, static_cast<int>(e.Key), e.ScanCode, e.Action, e.Mods);
        });

        m_Listener->SubscribeEvent<KeyReleasedEvent>([](const KeyReleasedEvent e)
        {
            ImGui_ImplGlfw_KeyCallback(e.GlfWWindow, static_cast<int>(e.Key), e.ScanCode, e.Action, e.Mods);
        });

        m_Listener->SubscribeEvent<KeyRepeatedEvent>([](const KeyRepeatedEvent e)
        {
            ImGui_ImplGlfw_KeyCallback(e.GlfWWindow, static_cast<int>(e.Key), e.ScanCode, e.Action, e.Mods);
        });

        m_Listener->SubscribeEvent<KeyTypedEvent>([](const KeyTypedEvent e)
        {
            ImGui_ImplGlfw_CharCallback(e.GlfWWindow, static_cast<unsigned int>(e.Key));
        });

        m_Listener->SubscribeEvent<MonitorEvent>([](const MonitorEvent e)
        {
            ImGui_ImplGlfw_MonitorCallback(e.Monitor, e.Event);
        });
    }

    ImGuiImplRenderLib::~ImGuiImplRenderLib()
    {
        ImGui_ImplGlfw_Shutdown();
    }

    void ImGuiImplRenderLib::NewFrame()
    {
        ImGui_ImplGlfw_NewFrame();

        const auto desc = m_MainWindow->GetSwapChain()->GetDesc();
        ImGuiImplDiligent::NewFrame(desc.Width, desc.Height, desc.PreTransform);
    }

    void ImGuiImplRenderLib::Render(Diligent::IDeviceContext* pCtx)
    {
        ImGuiImplDiligent::Render(pCtx);
    }

    void ImGuiImplRenderLib::EndFrame()
    {
        ImGuiImplDiligent::EndFrame();
    }
}
