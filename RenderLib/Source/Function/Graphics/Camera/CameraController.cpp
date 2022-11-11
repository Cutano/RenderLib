#include "CameraController.h"

#include "MathCamera.h"
#include "Platform/Input/InputManager.h"
#include "Utility/Event/EventListener.h"
#include "Utility/Event/Events.h"

namespace RL
{
	static void DisableMouse()
	{
		InputManager::Get().SetCursorMode(CursorMode::Locked);
	}

	static void EnableMouse()
	{
		InputManager::Get().SetCursorMode(CursorMode::Normal);
	}
	
    CameraController::CameraController(MathCamera* mathCamera) : m_MathCamera(mathCamera)
    {
        m_Listener = new EventListener();
        
        Init();

        m_Listener->SubscribeEvent<MouseScrolledEvent>([this](const MouseScrolledEvent& e)
        {
            if (InputManager::Get().IsMouseButtonDown(MouseButton::Right))
            {
                m_NormalSpeed += static_cast<float>(e.Y) * 0.3f * m_NormalSpeed;
                m_NormalSpeed = glm::clamp(m_NormalSpeed, MIN_SPEED, MAX_SPEED);
            }
            else
            {
                MouseZoom(static_cast<float>(e.Y) * 0.1f);
                UpdateCameraView();
            }

            return true;
        });
    }

    CameraController::~CameraController()
    {
        delete m_Listener;
    }

    void CameraController::Init()
    {
        constexpr glm::vec3 position = {0, 1, 0};
        m_Distance = distance(position, m_FocalPoint);

        m_Position = CalculatePosition();
        const glm::quat orientation = GetOrientation();
        m_Direction = eulerAngles(orientation) * (180.0f / glm::pi<float>());
		m_MathCamera->SetViewMatrix(inverse(translate(glm::mat4(1.0f), m_Position) * toMat4(orientation)));
    }

    void CameraController::Focus(const glm::vec3& focusPoint)
    {
        m_FocalPoint = focusPoint;
        m_CameraMode = CameraMode::Flycam;
        if (m_Distance > m_MinFocusDistance)
        {
            m_Distance -= m_Distance - m_MinFocusDistance;
            m_Position = m_FocalPoint - GetForwardDirection() * m_Distance;
        }
        m_Position = m_FocalPoint - GetForwardDirection() * m_Distance;
        UpdateCameraView();
    }

    void CameraController::OnUpdate(double ts)
    {
        const glm::vec2& mouse{ InputManager::Get().GetMouseX(), InputManager::Get().GetMouseY() };
		const glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.002f;

		if (m_IsActive)
		{
			if (InputManager::Get().IsMouseButtonDown(MouseButton::Right) && !InputManager::Get().IsKeyDown(KeyCode::LeftAlt))
			{
				m_CameraMode = CameraMode::Flycam;
				DisableMouse();
				const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;

				const float speed = GetCameraSpeed();

				if (InputManager::Get().IsKeyDown(KeyCode::Q))
					m_PositionDelta -= ts * speed * glm::vec3{ 0.f, yawSign, 0.f };
				if (InputManager::Get().IsKeyDown(KeyCode::E))
					m_PositionDelta += ts * speed * glm::vec3{ 0.f, yawSign, 0.f };
				if (InputManager::Get().IsKeyDown(KeyCode::S))
					m_PositionDelta -= ts * speed * m_Direction;
				if (InputManager::Get().IsKeyDown(KeyCode::W))
					m_PositionDelta += ts * speed * m_Direction;
				if (InputManager::Get().IsKeyDown(KeyCode::A))
					m_PositionDelta -= ts * speed * m_RightDirection;
				if (InputManager::Get().IsKeyDown(KeyCode::D))
					m_PositionDelta += ts * speed * m_RightDirection;

				constexpr float maxRate{ 0.12f };
				m_YawDelta += glm::clamp(yawSign * delta.x * RotationSpeed(), -maxRate, maxRate);
				m_PitchDelta += glm::clamp(delta.y * RotationSpeed(), -maxRate, maxRate);

				m_RightDirection = glm::cross(m_Direction, glm::vec3{ 0.f, yawSign, 0.f });

				m_Direction = glm::rotate(glm::normalize(glm::cross(glm::angleAxis(-m_PitchDelta, m_RightDirection),
					glm::angleAxis(-m_YawDelta, glm::vec3{ 0.f, yawSign, 0.f }))), m_Direction);

				const float distance = glm::distance(m_FocalPoint, m_Position);
				m_FocalPoint = m_Position + GetForwardDirection() * distance;
				m_Distance = distance;
			}
			else if (InputManager::Get().IsKeyDown(KeyCode::LeftAlt))
			{
				m_CameraMode = CameraMode::Arcball;

				if (InputManager::Get().IsMouseButtonDown(MouseButton::Middle))
				{
					DisableMouse();
					MousePan(delta);
				}
				else if (InputManager::Get().IsMouseButtonDown(MouseButton::Left))
				{
					DisableMouse();
					MouseRotate(delta);
				}
				else if (InputManager::Get().IsMouseButtonDown(MouseButton::Right))
				{
					DisableMouse();
					MouseZoom(delta.x + delta.y);
				}
				else
					EnableMouse();
			}
			else
			{
				EnableMouse();
			}
		}
		m_InitialMousePosition = mouse;

		m_Position += m_PositionDelta;
		m_Yaw += m_YawDelta;
		m_Pitch += m_PitchDelta;

		if (m_CameraMode == CameraMode::Arcball)
			m_Position = CalculatePosition();

		UpdateCameraView();
    }

    glm::vec3 CameraController::GetUpDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 CameraController::GetRightDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(1.f, 0.f, 0.f));
    }

    glm::vec3 CameraController::GetForwardDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::quat CameraController::GetOrientation() const
    {
        return glm::quat(glm::vec3(-m_Pitch - m_PitchDelta, -m_Yaw - m_YawDelta, 0.0f));
    }

    float CameraController::GetCameraSpeed() const
    {
        float speed = m_NormalSpeed;
        if (InputManager::Get().IsKeyDown(KeyCode::LeftControl))
            speed /= 2 - glm::log(m_NormalSpeed);
        if (InputManager::Get().IsKeyDown(KeyCode::LeftShift))
            speed *= 2 - glm::log(m_NormalSpeed);

        return glm::clamp(speed, MIN_SPEED, MAX_SPEED);
    }

    void CameraController::SetViewportSize(const uint32_t width, const uint32_t height) const
    {
	    m_MathCamera->SetPerspectiveProjectionMatrix(m_VerticalFOV, static_cast<float>(width), static_cast<float>(height), m_NearClip, m_FarClip);
    }

    void CameraController::UpdateCameraView()
    {
        const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;

        // Extra step to handle the problem when the camera direction is the same as the up vector
        const float cosAngle = glm::dot(GetForwardDirection(), GetUpDirection());
        if (cosAngle * yawSign > 0.99f)
            m_PitchDelta = 0.f;

        const glm::vec3 lookAt = m_Position + GetForwardDirection();
        m_Direction = glm::normalize(lookAt - m_Position);
        m_Distance = glm::distance(m_Position, m_FocalPoint);
        m_MathCamera->SetViewMatrix(glm::lookAt(m_Position, lookAt, glm::vec3{ 0.f, yawSign, 0.f }));

        //damping for smooth camera
        m_YawDelta *= 0.6f;
        m_PitchDelta *= 0.6f;
        m_PositionDelta *= 0.8f;
    }

    void CameraController::MousePan(const glm::vec2& delta)
    {
        auto [xSpeed, ySpeed] = PanSpeed();
        m_FocalPoint -= GetRightDirection() * delta.x * xSpeed * m_Distance;
        m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
    }

    void CameraController::MouseRotate(const glm::vec2& delta)
    {
        const float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
        m_YawDelta += yawSign * delta.x * RotationSpeed();
        m_PitchDelta += delta.y * RotationSpeed();
    }

    void CameraController::MouseZoom(float delta)
    {
        m_Distance -= delta * ZoomSpeed();
        const glm::vec3 forwardDir = GetForwardDirection();
        m_Position = m_FocalPoint - forwardDir * m_Distance;
        if (m_Distance < 1.0f)
        {
            m_FocalPoint += forwardDir * m_Distance;
            m_Distance = 1.0f;
        }
        m_PositionDelta += delta * ZoomSpeed() * forwardDir;
    }

    glm::vec3 CameraController::CalculatePosition() const
    {
        return m_FocalPoint - GetForwardDirection() * m_Distance + m_PositionDelta;
    }

    std::pair<float, float> CameraController::PanSpeed() const
    {
        // const float x = glm::min(float(m_ViewportWidth) / 1000.0f, 2.4f); // max = 2.4f
        const float x = glm::min(1.0f, 2.4f); // max = 2.4f
        const float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        // const float y = glm::min(float(m_ViewportHeight) / 1000.0f, 2.4f); // max = 2.4f
        const float y = glm::min(1.0f, 2.4f); // max = 2.4f
        const float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { xFactor, yFactor };
    }

    float CameraController::RotationSpeed() const
    {
        return 0.3f;
    }

    float CameraController::ZoomSpeed() const
    {
        float distance = m_Distance * 0.2f;
        distance = glm::max(distance, 0.0f);
        float speed = distance * distance;
        speed = glm::min(speed, 50.0f); // max speed = 50
        return speed;
    }
}
