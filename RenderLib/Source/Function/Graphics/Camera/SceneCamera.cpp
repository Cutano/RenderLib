#include "SceneCamera.h"

#include <Texture.h>
#include <TextureView.h>
#include <RefCntAutoPtr.hpp>

#include "RenderDevice.h"
#include "Function/Graphics/GraphicEngine.h"

namespace RL
{
    SceneCamera::SceneCamera(const uint16_t index, uint32_t width, uint32_t height)
        : Camera(width, height), m_Index(index)
    {
        m_ColorTextureName = std::string("SceneCameraFinalColorTexture").append(std::to_string(index + 1));

        CreateRTV(width, height);
    }

    SceneCamera::~SceneCamera()
    {

    }

    void SceneCamera::PrepareForPresent()
    {
        
    }

    void SceneCamera::ResizeViewport(const uint32_t width, const uint32_t height)
    {
        SetViewportSize(width, height);
        CreateRTV(width, height);
    }

    Diligent::ITextureView* SceneCamera::GetRenderTargetView()
    {
        return m_RenderTargetView;
    }

    void SceneCamera::CreateRTV(uint32_t width, uint32_t height)
    {
        // Create offscreen render target
        Diligent::TextureDesc RTColorDesc;
        RTColorDesc.Name      = m_ColorTextureName.c_str();
        RTColorDesc.Type      = Diligent::RESOURCE_DIM_TEX_2D;
        RTColorDesc.Width     = width;
        RTColorDesc.Height    = height;
        RTColorDesc.MipLevels = 1;
        RTColorDesc.Format    = Diligent::TEX_FORMAT_RGBA8_UNORM;
        // The render target can be bound as a shader resource and as a render target
        RTColorDesc.BindFlags = Diligent::BIND_SHADER_RESOURCE | Diligent::BIND_RENDER_TARGET;
        // Define optimal clear value
        RTColorDesc.ClearValue.Format   = RTColorDesc.Format;
        RTColorDesc.ClearValue.Color[0] = 0.350f;
        RTColorDesc.ClearValue.Color[1] = 0.350f;
        RTColorDesc.ClearValue.Color[2] = 0.350f;
        RTColorDesc.ClearValue.Color[3] = 1.f;

        Diligent::RefCntAutoPtr<Diligent::ITexture> colorRT;
        GraphicEngine::Get().GetDevice()->CreateTexture(RTColorDesc, nullptr, &colorRT);
        m_RenderTargetView = colorRT->GetDefaultView(Diligent::TEXTURE_VIEW_RENDER_TARGET);
    }
}
