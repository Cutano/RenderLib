#include "GraphicEngine.h"

#include "EngineFactoryD3D12.h"

namespace RL
{
    void GraphicEngine::Init()
    {
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

    void GraphicEngine::Shutdown()
    {
        
    }
}
