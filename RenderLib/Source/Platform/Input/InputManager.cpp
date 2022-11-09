#include "InputManager.h"
#include "Platform/Window/Window.h"
#include "Platform/Window/WindowManager.h"
#include "Utility/Event/Events.h"
#include "Utility/Event/EventBus.h"
#include "Utility/Event/EventListener.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <ranges>
#include <set>

namespace RL
{
    void InputManager::Init()
    {
        m_Listener = new EventListener();

        m_Listener->SubscribeEvent<KeyPressedEvent>([this](const KeyPressedEvent& e)
        {
            m_PressedKeys.insert(e.Key);

            for (const auto& [name, hotkey] : m_HotKeyRegistry)
            {
                if (hotkey.TriggerKey == e.Key && IsKeyAllDown(hotkey.HoldKeys))
                {
                    EventBus::Get().SpreadEvent<HotkeyEvent>({name});
                }
            }
        });

        m_Listener->SubscribeEvent<MouseButtonPressedEvent>([this](const MouseButtonPressedEvent& e)
        {
            m_PressedButtons.insert(e.Button);
        });
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

        // Finish recording keys
        m_PrevPressedKeys = m_PressedKeys;
        m_PrevPressedButtons = m_PressedButtons;
        m_PressedKeys.clear();
        m_PressedButtons.clear();
    }

    void InputManager::Shutdown()
    {
        delete m_Listener;
    }

    bool InputManager::IsKeyDown(KeyCode keycode)
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

    bool InputManager::IsKeyAllDown(const std::set<KeyCode>& keyCodes)
    {
        for (const auto& keyCode : keyCodes)
        {
            if (!IsKeyDown(keyCode))
            {
                return false;
            }
        }

        return true;
    }

    bool InputManager::IsKeyAllDown(const std::initializer_list<KeyCode> keyCodes)
    {
        for (const auto& keyCode : keyCodes)
        {
            if (!IsKeyDown(keyCode))
            {
                return false;
            }
        }

        return true;
    }

    bool InputManager::IsMouseButtonDown(MouseButton button)
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

    bool InputManager::HasKeyPressed(const KeyCode keycode) const
    {
        return m_PrevPressedKeys.contains(keycode);
    }

    bool InputManager::HasMouseButtonPressed(const MouseButton button) const
    {
        return m_PrevPressedButtons.contains(button);
    }

    bool InputManager::HasHotKeyTriggered(const HotKey& hotKey)
    {
        if (hotKey.HoldKeys.empty() || !HasKeyPressed(hotKey.TriggerKey))
        {
            return false;
        }

        if (!IsKeyAllDown(hotKey.HoldKeys))
        {
            return false;
        }

        return true;
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

    HotKey& InputManager::GetHotKey(const std::wstring& name)
    {
        return m_HotKeyRegistry[name];
    }

    bool InputManager::IsHotKeyRegistered(const HotKey& hotKey) const
    {
        return m_HotKeyRegistry.contains(hotKey.Name);
    }

    void InputManager::RegisterHotKey(const HotKey& hotKey)
    {
        m_HotKeyRegistry[hotKey.Name] = hotKey;
    }

    void InputManager::UnregisterHotKey(const HotKey& hotKey)
    {
        m_HotKeyRegistry.erase(hotKey.Name);
    }

    void InputManager::UnregisterAllHotKey()
    {
        m_HotKeyRegistry.clear();
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
