#include "GraphicEngine.h"
#include "Platform/Window/WindowManager.h"
#include "Platform/Window/Window.h"

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
                    Log::GraphicsLogger()->info("Engine: {}", msg);
                    break;
                case Diligent::DEBUG_MESSAGE_SEVERITY_WARNING:
                    Log::GraphicsLogger()->warn("Engine: {}", msg);
                    break;
                case Diligent::DEBUG_MESSAGE_SEVERITY_ERROR:
                    Log::GraphicsLogger()->error("Engine: {}", msg);
                    break;
                case Diligent::DEBUG_MESSAGE_SEVERITY_FATAL_ERROR:
                    Log::GraphicsLogger()->critical("Engine: {}", msg);
                    break;
                }
            }
            else
            {
                switch (severity)
                {
                case Diligent::DEBUG_MESSAGE_SEVERITY_INFO:
                    Log::GraphicsLogger()->info("Engine: {}, in function {}, file {}, line {}", msg, function, file,
                                                std::to_string(line).c_str());
                    break;
                case Diligent::DEBUG_MESSAGE_SEVERITY_WARNING:
                    Log::GraphicsLogger()->warn("Engine: {}, in function {}, file {}, line {}", msg, function, file,
                                                std::to_string(line).c_str());
                    break;
                case Diligent::DEBUG_MESSAGE_SEVERITY_ERROR:
                    Log::GraphicsLogger()->error("Engine: {}, in function {}, file {}, line {}", msg, function, file,
                                                 std::to_string(line).c_str());
                    break;
                case Diligent::DEBUG_MESSAGE_SEVERITY_FATAL_ERROR:
                    Log::GraphicsLogger()->critical("Engine: {}, in function {}, file {}, line {}", msg, function, file,
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
        engineCI.EnableValidation = false;
        engineCI.D3D12ValidationFlags = Diligent::D3D12_VALIDATION_FLAG_NONE;
        engineCI.SetValidationLevel(Diligent::VALIDATION_LEVEL_DISABLED);
#endif
        
        m_EngineFactory = Diligent::GetEngineFactoryD3D12();
        m_EngineFactory->CreateDeviceAndContextsD3D12(createInfo, &m_RenderDevice, &m_DeviceContext);
    }

    void GraphicEngine::Update()
    {
        
    }

    void GraphicEngine::Shutdown()
    {
        
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

        Diligent::ISwapChain* swapChain;
        const Diligent::Win32NativeWindow window(mainWindow->GetHwnd());

        m_EngineFactory->CreateSwapChainD3D12(m_RenderDevice, m_DeviceContext, swapChainDesc, {}, window, &swapChain);
        mainWindow->SetSwapChain(swapChain);
    }
}
