#pragma once

#include "Utility/Math.h"

namespace RL
{
    class MathCamera
    {
    public:
        enum class ProjectionType { Perspective = 0, Orthographic = 1 };
        
        MathCamera() = default;
        MathCamera(const glm::mat4& projection, const glm::mat4& unReversedProjection);
        MathCamera(const float degFov, const float width, const float height, const float nearP, const float farP);
        ~MathCamera() = default;

        [[nodiscard]] const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        [[nodiscard]] const glm::mat4& GetUnReversedProjectionMatrix() const { return m_UnReversedProjectionMatrix; }
        [[nodiscard]] const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        [[nodiscard]] glm::mat4 GetViewProjection() const { return GetProjectionMatrix() * m_ViewMatrix; }
        [[nodiscard]] glm::mat4 GetUnReversedViewProjection() const { return GetUnReversedProjectionMatrix() * m_ViewMatrix; }

        void SetProjectionMatrix(const glm::mat4 projection, const glm::mat4 unReversedProjection)
        {
            m_ProjectionMatrix = projection;
            m_UnReversedProjectionMatrix = unReversedProjection;
        }

        void SetPerspectiveProjectionMatrix(const float radFov, const float width, const float height, const float nearP, const float farP)
        {
            m_ProjectionMatrix = glm::perspectiveFov(radFov, width, height, farP, nearP);
            m_UnReversedProjectionMatrix = glm::perspectiveFov(radFov, width, height, nearP, farP);
        }

        void SetOrthoProjectionMatrix(const float width, const float height, const float nearP, const float farP)
        {
            m_ProjectionMatrix = glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, farP, nearP);
            m_UnReversedProjectionMatrix = glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, nearP, farP);
        }

        void SetViewMatrix(const glm::mat4& view)
        {
            m_ViewMatrix = view;
        }

        void SetPerspective(float verticalFOV, float nearClip = 0.1f, float farClip = 1000.0f);
        void SetOrthographic(float size, float nearClip = -1.0f, float farClip = 1.0f);
        void SetViewportSize(uint32_t width, uint32_t height);

        void SetDegPerspectiveVerticalFOV(const float degVerticalFov) { m_DegPerspectiveFOV = degVerticalFov; }
        void SetRadPerspectiveVerticalFOV(const float degVerticalFov) { m_DegPerspectiveFOV = glm::degrees(degVerticalFov); }
        float GetDegPerspectiveVerticalFOV() const { return m_DegPerspectiveFOV; }
        float GetRadPerspectiveVerticalFOV() const { return glm::radians(m_DegPerspectiveFOV); }
        void SetPerspectiveNearClip(const float nearClip) { m_PerspectiveNear = nearClip; }
        float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
        void SetPerspectiveFarClip(const float farClip) { m_PerspectiveFar = farClip; }
        float GetPerspectiveFarClip() const { return m_PerspectiveFar; }

        void SetOrthographicSize(const float size) { m_OrthographicSize = size; }
        float GetOrthographicSize() const { return m_OrthographicSize; }
        void SetOrthographicNearClip(const float nearClip) { m_OrthographicNear = nearClip; }
        float GetOrthographicNearClip() const { return m_OrthographicNear; }
        void SetOrthographicFarClip(const float farClip) { m_OrthographicFar = farClip; }
        float GetOrthographicFarClip() const { return m_OrthographicFar; }

        void SetProjectionType(ProjectionType type) { m_ProjectionType = type; }
        ProjectionType GetProjectionType() const { return m_ProjectionType; }

    private:
        glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
        glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
        //Currently only needed for shadow maps and ImGuizmo
        glm::mat4 m_UnReversedProjectionMatrix = glm::mat4(1.0f);

        ProjectionType m_ProjectionType = ProjectionType::Perspective;

        float m_DegPerspectiveFOV = 45.0f;
        float m_PerspectiveNear = 0.1f, m_PerspectiveFar = 1000.0f;

        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
    };
}
