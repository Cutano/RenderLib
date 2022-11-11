#include "GraphicEngine.h"
#include "Platform/Window/WindowManager.h"
#include "Platform/Window/Window.h"
#include "Camera/SceneCamera.h"
#include "ImGui/ImGuiImplRenderLib.h"

#include <EngineFactoryD3D12.h>

namespace RL
{
    void GraphicEngine::Init()
    {
        Diligent::SetDebugMessageCallback([](Diligent::DEBUG_MESSAGE_SEVERITY severity,
                                             const char* msg,
                                             const char* function,
                                             const char* file,
                                             int line)
        {
            if (function == nullptr)
            {
                switch (severity)
                {
                case Diligent::DEBUG_MESSAGE_SEVERITY_INFO:
                    Log::GraphicsEngineLogger()->info("{}", msg);
                    break;
                case Diligent::DEBUG_MESSAGE_SEVERITY_WARNING:
                    Log::GraphicsEngineLogger()->warn("{}", msg);
                    break;
                case Diligent::DEBUG_MESSAGE_SEVERITY_ERROR:
                    Log::GraphicsEngineLogger()->error("{}", msg);
                    break;
                case Diligent::DEBUG_MESSAGE_SEVERITY_FATAL_ERROR:
                    Log::GraphicsEngineLogger()->critical("{}", msg);
                    break;
                }
            }
            else
            {
                switch (severity)
                {
                case Diligent::DEBUG_MESSAGE_SEVERITY_INFO:
                    Log::GraphicsEngineLogger()->info("{}, in function {}, file {}, line {}", msg, function, file,
                                                std::to_string(line).c_str());
                    break;
                case Diligent::DEBUG_MESSAGE_SEVERITY_WARNING:
                    Log::GraphicsEngineLogger()->warn("{}, in function {}, file {}, line {}", msg, function, file,
                                                std::to_string(line).c_str());
                    break;
                case Diligent::DEBUG_MESSAGE_SEVERITY_ERROR:
                    Log::GraphicsEngineLogger()->error("{}, in function {}, file {}, line {}", msg, function, file,
                                                 std::to_string(line).c_str());
                    break;
                case Diligent::DEBUG_MESSAGE_SEVERITY_FATAL_ERROR:
                    Log::GraphicsEngineLogger()->critical("{}, in function {}, file {}, line {}", msg, function, file,
                                                    std::to_string(line).c_str());
                    break;
                }
            }
        });
        
        Diligent::EngineD3D12CreateInfo createInfo;
        
#ifdef _DEBUG
        createInfo.EnableValidation = true;
        createInfo.D3D12ValidationFlags = Diligent::D3D12_VALIDATION_FLAG_ENABLE_GPU_BASED_VALIDATION;
        createInfo.SetValidationLevel(Diligent::VALIDATION_LEVEL_2);
#else
        createInfo.EnableValidation = false;
        createInfo.D3D12ValidationFlags = Diligent::D3D12_VALIDATION_FLAG_NONE;
        createInfo.SetValidationLevel(Diligent::VALIDATION_LEVEL_DISABLED);
#endif
        
        m_EngineFactory = Diligent::GetEngineFactoryD3D12();
        m_EngineFactory->CreateDeviceAndContextsD3D12(createInfo, &m_RenderDevice, &m_DeviceContext);

        for (uint16_t i = 0; i < static_cast<uint16_t>(m_SceneCameras.size()); ++i)
        {
            m_SceneCameras.at(i) = new SceneCamera(i, 1440, 900);
        }
    }

    void GraphicEngine::Update()
    {
        m_ImGuiImpl->NewFrame();
    }

    void GraphicEngine::Render()
    {
        const auto mainWindow = WindowManager::Get().GetMainWindow();
        auto swapChain = mainWindow->GetSwapChain();

        auto* rtv = swapChain->GetCurrentBackBufferRTV();
        auto* dsv = swapChain->GetDepthBufferDSV();
        
        constexpr float clearColor[] = {0.45f, 0.55f, 0.60f, 1.00f};

        m_DeviceContext->SetRenderTargets(1, &rtv, dsv, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_DeviceContext->ClearRenderTarget(
            WindowManager::Get().GetMainWindow()->GetSwapChain()->GetCurrentBackBufferRTV(),
            clearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        
        m_ImGuiImpl->Render(m_DeviceContext);
        m_ImGuiImpl->EndFrame();
    }

    void GraphicEngine::Shutdown()
    {
        delete m_ImGuiImpl;

        m_DeviceContext->Flush();
        m_DeviceContext->Release();
        m_DeviceContext = nullptr;

        for (uint16_t i = 0; i < static_cast<uint16_t>(m_SceneCameras.size()); ++i)
        {
            delete m_SceneCameras.at(i);
        }

        m_RenderDevice->Release();
        m_RenderDevice = nullptr;

        m_EngineFactory->Release();
        m_EngineFactory = nullptr;
    }

    void GraphicEngine::AttachMainWindow()
    {
        const auto mainWindow = WindowManager::Get().GetMainWindow();

        Diligent::SwapChainDesc swapChainDesc;
        swapChainDesc.Usage = Diligent::SWAP_CHAIN_USAGE_RENDER_TARGET;
        swapChainDesc.ColorBufferFormat = Diligent::TEX_FORMAT_RGBA8_UNORM;
        swapChainDesc.Width = mainWindow->GetWidth();
        swapChainDesc.Height = mainWindow->GetHeight();
        swapChainDesc.BufferCount = 2;
        swapChainDesc.DefaultDepthValue = 0;

        Diligent::RefCntAutoPtr<Diligent::ISwapChain> swapChain;
        const Diligent::Win32NativeWindow window(mainWindow->GetHwnd());

        m_EngineFactory->CreateSwapChainD3D12(m_RenderDevice, m_DeviceContext, swapChainDesc, {}, window, &swapChain);
        mainWindow->SetSwapChain(swapChain);
    }

    void GraphicEngine::AttachGuiBackend()
    {
        const auto mainWindow = WindowManager::Get().GetMainWindow();
        m_ImGuiImpl = new ImGuiImplRenderLib(mainWindow, m_RenderDevice);
    }

    void GraphicEngine::AttachWindow(const std::shared_ptr<Window>& window)
    {
        Diligent::SwapChainDesc swapChainDesc;
        swapChainDesc.Usage = Diligent::SWAP_CHAIN_USAGE_RENDER_TARGET;
        swapChainDesc.ColorBufferFormat = Diligent::TEX_FORMAT_RGBA8_UNORM;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.DefaultDepthValue = 0;
        
        Diligent::RefCntAutoPtr<Diligent::ISwapChain> swapChain;
        const Diligent::Win32NativeWindow nativeWindow(window->GetHwnd());

        m_EngineFactory->CreateSwapChainD3D12(m_RenderDevice, m_DeviceContext, swapChainDesc, {}, nativeWindow, &swapChain);
        window->SetSwapChain(swapChain);
    }

    void GraphicEngine::AttachRawWindow(HWND hwnd)
    {
        Diligent::SwapChainDesc swapChainDesc;
        swapChainDesc.Usage = Diligent::SWAP_CHAIN_USAGE_RENDER_TARGET;
        swapChainDesc.ColorBufferFormat = Diligent::TEX_FORMAT_RGBA8_UNORM;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.DefaultDepthValue = 0;
        
        Diligent::ISwapChain* swapChain;
        const Diligent::Win32NativeWindow window(hwnd);

        m_EngineFactory->CreateSwapChainD3D12(m_RenderDevice, m_DeviceContext, swapChainDesc, {}, window, &swapChain);
    }

    SceneCamera* GraphicEngine::GetSceneCamera(const uint16_t index)
    {
        return m_SceneCameras.at(index);
    }

    Diligent::IDeviceContext* GraphicEngine::GetDeviceContext() const
    {
        return m_DeviceContext;
    }

    Diligent::IRenderDevice* GraphicEngine::GetDevice() const
    {
        return m_RenderDevice;
    }
}
