#include "MathCamera.h"

namespace RL
{
    MathCamera::MathCamera(const glm::mat4& projection, const glm::mat4& unReversedProjection) :
    m_ProjectionMatrix(projection), m_UnReversedProjectionMatrix(unReversedProjection)
    {
    }

    MathCamera::MathCamera(const float degFov, const float width, const float height, const float nearP,
        const float farP) :
    m_ProjectionMatrix(glm::perspectiveFov(glm::radians(degFov), width, height, farP, nearP)),
    m_UnReversedProjectionMatrix(glm::perspectiveFov(glm::radians(degFov), width, height, nearP, farP))
    {
    }

    void MathCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
    {
        m_ProjectionType = ProjectionType::Perspective;
        m_DegPerspectiveFOV = verticalFOV;
        m_PerspectiveNear = nearClip;
        m_PerspectiveFar = farClip;
    }

    void MathCamera::SetOrthographic(float size, float nearClip, float farClip)
    {
        m_ProjectionType = ProjectionType::Orthographic;
        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farClip;
    }

    void MathCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        switch (m_ProjectionType)
        {
        case ProjectionType::Perspective:
            SetPerspectiveProjectionMatrix(glm::radians(m_DegPerspectiveFOV), static_cast<float>(width), static_cast<float>(height), m_PerspectiveNear, m_PerspectiveFar);
            break;
        case ProjectionType::Orthographic:
            const float aspect = static_cast<float>(width) / static_cast<float>(height);
            const float _width = m_OrthographicSize * aspect;
            const float _height = m_OrthographicSize;
            SetOrthoProjectionMatrix(_width, _height, m_OrthographicNear, m_OrthographicFar);
            break;
        }
    }
}
