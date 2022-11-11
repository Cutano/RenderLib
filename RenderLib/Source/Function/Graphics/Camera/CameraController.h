#pragma once
#include <Utility/Math.h>

namespace RL
{
    class EventListener;
    class MathCamera;

    enum class CameraMode
    {
        None, Flycam, Arcball
    };
    
    class CameraController
    {
    public:
        explicit CameraController(MathCamera* mathCamera);
        ~CameraController();

        void Init();
        void Focus(const glm::vec3& focusPoint);
        void OnUpdate(double ts);

        [[nodiscard]] bool IsActive() const { return m_IsActive; }
        void SetActive(const bool active) { m_IsActive = active; }

        [[nodiscard]] CameraMode GetCurrentMode() const { return m_CameraMode; }

        [[nodiscard]] float GetDistance() const { return m_Distance; }
        void SetDistance(const float distance) { m_Distance = distance; }

        [[nodiscard]] const glm::vec3& GetFocalPoint() const { return m_FocalPoint; }
        
        void SetViewportSize(uint32_t width, uint32_t height) const;

        [[nodiscard]] glm::vec3 GetUpDirection() const;
        [[nodiscard]] glm::vec3 GetRightDirection() const;
        [[nodiscard]] glm::vec3 GetForwardDirection() const;

        [[nodiscard]] const glm::vec3& GetPosition() const { return m_Position; }
        [[nodiscard]] glm::quat GetOrientation() const;

        [[nodiscard]] float GetPitch() const { return m_Pitch; }
        [[nodiscard]] float GetYaw() const { return m_Yaw; }
        [[nodiscard]] float GetCameraSpeed() const;

    private:
        void UpdateCameraView();

        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);
        void MouseZoom(float delta);

        glm::vec3 CalculatePosition() const;

        std::pair<float, float> PanSpeed() const;
        float RotationSpeed() const;
        float ZoomSpeed() const;

        MathCamera* m_MathCamera{nullptr};
        EventListener* m_Listener{nullptr};

        CameraMode m_CameraMode{CameraMode::Arcball};

        bool m_IsActive{false};
        bool m_Panning{false}, m_Rotating{false};

        glm::vec3 m_Position{0}, m_Direction{}, m_FocalPoint{0};
        glm::vec2 m_InitialMousePosition{};
        glm::vec3 m_InitialFocalPoint{}, m_InitialRotation{};
        glm::vec3 m_PositionDelta{};
        glm::vec3 m_RightDirection{};

        float m_Distance{0.0f};
        float m_NormalSpeed{0.002f};
        float m_MinFocusDistance{100.0f};
        float m_Pitch{0}, m_Yaw{0};
        float m_PitchDelta{}, m_YawDelta{};

        // Perspective projection params
        float m_VerticalFOV{glm::pi<float>() / 3.0f}, m_AspectRatio{16.0f / 9.0f}, m_NearClip{0.01f}, m_FarClip{1000.0f};

        constexpr static float MIN_SPEED{0.0005f}, MAX_SPEED{2.0f};
    };
}
