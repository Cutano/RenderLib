#include "InputManager.h"
#include "Platform/Window/Window.h"
#include "Platform/Window/WindowManager.h"
#include "Utility/Event/EventBus.h"
#include "Utility/Event/Events.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <ranges>
#include <set>

namespace RL
{
    void InputManager::Init()
    {
    }

    void InputManager::Update()
    {
        // Cleanup disconnected controller
        for (auto it = m_Controllers.begin(); it != m_Controllers.end(); )
        {
            int id = it->first;
            if (glfwJoystickPresent(id) != GLFW_TRUE)
                it = m_Controllers.erase(it);
            else
                it++;
        }

        // Update controllers
        for (int id = GLFW_JOYSTICK_1; id < GLFW_JOYSTICK_LAST; id++)
        {
            if (glfwJoystickPresent(id) == GLFW_TRUE)
            {
                Controller& controller = m_Controllers[id];
                controller.ID = id;
                controller.Name = glfwGetJoystickName(id);

                int buttonCount;
                const unsigned char* buttons = glfwGetJoystickButtons(id, &buttonCount);
                for (int i = 0; i < buttonCount; i++)
                    controller.ButtonStates[i] = buttons[i] == GLFW_PRESS;

                int axisCount;
                const float* axes = glfwGetJoystickAxes(id, &axisCount);
                for (int i = 0; i < axisCount; i++)
                    controller.AxisStates[i] = axes[i];

                int hatCount;
                const unsigned char* hats = glfwGetJoystickHats(id, &hatCount);
                for (int i = 0; i < hatCount; i++)
                    controller.HatStates[i] = hats[i];
            }
        }
    }

    bool InputManager::IsKeyPressed(KeyCode keycode)
    {
        ImGuiContext* context = ImGui::GetCurrentContext();
        
        for (ImGuiViewport* viewport : context->Viewports)
        {
            GLFWwindow* windowHandle = WindowManager::Get().GetMainWindow()->GetGlfWWindow();
            if (!windowHandle)
                continue;
            const auto state = glfwGetKey(windowHandle, static_cast<int32_t>(keycode));
            if (state == GLFW_PRESS || state == GLFW_REPEAT)
            {
                return true;
            }
        }

        return false;
    }

    bool InputManager::IsMouseButtonPressed(MouseButton button)
    {
        ImGuiContext* context = ImGui::GetCurrentContext();
        
        for (ImGuiViewport* viewport : context->Viewports)
        {
            GLFWwindow* windowHandle = WindowManager::Get().GetMainWindow()->GetGlfWWindow();
            if (!windowHandle)
                continue;
            const auto state = glfwGetMouseButton(windowHandle, static_cast<int32_t>(button));
            if (state == GLFW_PRESS || state == GLFW_REPEAT)
            {
                return true;
            }
        }

        return false;
    }

    bool InputManager::IsHotKeyTriggered(const std::set<KeyCode>& keycodes)
    {
        bool res = true;
        
        for (const auto& keycode : keycodes)
        {
            res &= IsKeyPressed(keycode);
            if (!res)
            {
                return false;
            }
        }

        return res;
    }

    bool InputManager::IsHotKeyTriggered(const std::initializer_list<KeyCode> keycodes)
    {
        bool res = true;
        
        for (const auto& keycode : keycodes)
        {
            res &= IsKeyPressed(keycode);
            if (!res)
            {
                return false;
            }
        }

        return res;
    }

    float InputManager::GetMouseX()
    {
        auto [x, y] = GetMousePosition();
        return x;
    }

    float InputManager::GetMouseY()
    {
        auto [x, y] = GetMousePosition();
        return y;
    }

    std::pair<float, float> InputManager::GetMousePosition()
    {
        double x, y;
        glfwGetCursorPos(WindowManager::Get().GetMainWindow()->GetGlfWWindow(), &x, &y);
        return { static_cast<float>(x), static_cast<float>(y) };
    }

    void InputManager::SetCursorMode(CursorMode mode)
    {
        glfwSetInputMode(WindowManager::Get().GetMainWindow()->GetGlfWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL + static_cast<int>(mode));
    }

    CursorMode InputManager::GetCursorMode()
    {
        return static_cast<CursorMode>(
            glfwGetInputMode(WindowManager::Get().GetMainWindow()->GetGlfWWindow(), GLFW_CURSOR) - GLFW_CURSOR_NORMAL);
    }

    bool InputManager::IsControllerPresent(const int id)
    {
        return m_Controllers.contains(id);
    }

    std::vector<int> InputManager::GetConnectedControllerIDs()
    {
        std::vector<int> ids;
        ids.reserve(m_Controllers.size());
        for (const auto id : m_Controllers | std::views::keys)
        {
            ids.emplace_back(id);
        }

        return ids;
    }

    const Controller* InputManager::GetController(const int id)
    {
        if (!IsControllerPresent(id)) return nullptr;

        return &m_Controllers.at(id);
    }

    std::string_view InputManager::GetControllerName(const int id)
    {
        if (!IsControllerPresent(id)) return {};

        return m_Controllers.at(id).Name;
    }

    bool InputManager::IsControllerButtonPressed(const int controllerID, const int button)
    {
        if (!IsControllerPresent(controllerID)) return false;

        const auto& [
            ID,
            Name,
            ButtonStates,
            AxisStates,
            HatStates
        ] = m_Controllers.at(controllerID);
        
        if (!ButtonStates.contains(button)) return false;

        return ButtonStates.at(button);
    }

    float InputManager::GetControllerAxis(const int controllerID, const int axis)
    {
        if (!IsControllerPresent(controllerID)) return 0.0f;

        const auto& [
            ID,
            Name,
            ButtonStates,
            AxisStates,
            HatStates
        ] = m_Controllers.at(controllerID);
        
        if (!AxisStates.contains(axis)) return 0.0f;

        return AxisStates.at(axis);
    }

    uint8_t InputManager::GetControllerHat(const int controllerID, const int hat)
    {
        if (!IsControllerPresent(controllerID)) return 0;

        const auto& [
            ID,
            Name,
            ButtonStates,
            AxisStates,
            HatStates
        ] = m_Controllers.at(controllerID);
        
        if (!HatStates.contains(hat)) return 0;

        return HatStates.at(hat);
    }
}
