#include "Camera.h"

#include "MathCamera.h"
#include "CameraController.h"

namespace RL
{
    Camera::Camera(uint32_t width, uint32_t height) 
    {
        m_MathCamera = new MathCamera();
        m_Controller = new CameraController(m_MathCamera);

        m_Controller->SetViewportSize(width, height);
    }

    Camera::~Camera()
    {
        delete m_MathCamera;
        delete m_Controller;
    }

    void Camera::ActiveController(const bool active) const
    {
        m_Controller->SetActive(active);
    }

    void Camera::SetViewportSize(const uint32_t width, const uint32_t height) const
    {
        m_Controller->SetViewportSize(width, height);
    }
}
