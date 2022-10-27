#include <ImGuiDiligentRenderer.hpp>
#include <DeviceContext.h>
#include <backends/imgui_impl_glfw.h>
#include <SwapChain.h>

#include "ImGuiImplRenderLib.h"
#include "Utility/Event/EventListener.h"
#include "Utility/Event/Events.h"
#include "Platform/Window/Window.h"
#include "Platform/Window/WindowManager.h"
#include "Function/Graphics/GraphicEngine.h"

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

        ImGuiIO& io = ImGui::GetIO();

        io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
        io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();

            platform_io.Renderer_CreateWindow = [](ImGuiViewport* viewport)
            {
                const auto glfwWindow = static_cast<GLFWwindow*>(viewport->PlatformHandle);

                const auto window = WindowManager::Get().CreateFromRawGlfwWindow(glfwWindow);
                GraphicEngine::Get().AttachWindow(window);
            };

            platform_io.Renderer_DestroyWindow = [](ImGuiViewport* viewport)
            {
                HWND hwnd = viewport->PlatformHandleRaw ? static_cast<HWND>(viewport->PlatformHandleRaw) : static_cast<HWND>(viewport->PlatformHandle);

                WindowManager::Get().DestroyWindow(hwnd);
            };

            platform_io.Renderer_SetWindowSize = [](ImGuiViewport* viewport, ImVec2 size)
            {
                HWND hwnd = viewport->PlatformHandleRaw ? static_cast<HWND>(viewport->PlatformHandleRaw) : static_cast<HWND>(viewport->PlatformHandle);

                WindowManager::Get().GetWindow(hwnd)->Resize(static_cast<int>(size.x), static_cast<int>(size.y));
            };

            platform_io.Renderer_RenderWindow = [](ImGuiViewport* viewport, void* render_arg)
            {
                HWND hwnd = viewport->PlatformHandleRaw ? static_cast<HWND>(viewport->PlatformHandleRaw) : static_cast<HWND>(viewport->PlatformHandle);

                if (!hwnd) return;
                
                const auto window = WindowManager::Get().GetWindow(hwnd);
                const auto swapChain = window->GetSwapChain();
                const auto desc = window->GetSwapChain()->GetDesc();
                
                auto* rtv = swapChain->GetCurrentBackBufferRTV();
                auto* dsv = swapChain->GetDepthBufferDSV();
                
                constexpr float clearColor[] = {0.45f, 0.55f, 0.60f, 1.00f};
                
                const auto& context = GraphicEngine::Get().GetDeviceContext();
                
                context->SetRenderTargets(1, &rtv, dsv, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
                context->ClearRenderTarget(rtv, clearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
                
                static_cast<Diligent::ImGuiDiligentRenderer*>(render_arg)->RenderViewportDrawData(context, viewport->DrawData, desc.Width, desc.Height, desc.PreTransform);
            };

            platform_io.Renderer_SwapBuffers = [](ImGuiViewport* viewport, void* render_arg)
            {

            };
        }
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
        ImGui::RenderPlatformWindowsDefault(nullptr, m_pRenderer.get());
    }

    void ImGuiImplRenderLib::EndFrame()
    {
        ImGuiImplDiligent::
        EndFrame();
        ImGui::UpdatePlatformWindows();
    }

    void ImGuiImplRenderLib::RenderWindow(ImGuiViewport* viewport)
    {
    }
}
