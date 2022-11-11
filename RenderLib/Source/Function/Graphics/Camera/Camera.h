#pragma once
#include <cstdint>

namespace RL
{
    class MathCamera;
    class CameraController;
    
    class Camera
    {
    public:
        explicit Camera(uint32_t width, uint32_t height);
        virtual ~Camera();

        void virtual ActiveController(bool active) const;
        
    protected:
        void virtual SetViewportSize(uint32_t width, uint32_t height) const;
        
        MathCamera* m_MathCamera {nullptr};
        CameraController* m_Controller {nullptr};
    };
}
