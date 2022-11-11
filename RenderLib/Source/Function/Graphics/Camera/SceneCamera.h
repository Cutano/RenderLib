#pragma once
#include "Camera.h"
#include <string>

namespace Diligent
{
    struct ITextureView;
}

namespace RL
{
    class SceneCamera final : public Camera
    {
    public:
        SceneCamera(uint16_t index, uint32_t width, uint32_t height);
        ~SceneCamera() override;

        void PrepareForPresent();
        void ResizeViewport(uint32_t width, uint32_t height);
        
        [[nodiscard]] Diligent::ITextureView* GetRenderTargetView();

    private:
        void CreateRTV(uint32_t width, uint32_t height);
        
        uint16_t m_Index;

        Diligent::ITextureView* m_RenderTargetView {nullptr};
        std::string m_ColorTextureName;
    };
}
