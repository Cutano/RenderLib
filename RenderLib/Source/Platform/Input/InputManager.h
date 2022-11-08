#pragma once

#include "Utility/KeyCodes.h"

#include <utility>
#include <vector>
#include <string>
#include <map>
#include <set>

namespace RL
{
    class EventListener;
    
    struct Controller
    {
        int ID;
        std::string Name;
        std::map<int, bool> ButtonStates;
        std::map<int, float> AxisStates;
        std::map<int, uint8_t> HatStates;
    };
    
    class InputManager
    {
    public:
        static InputManager& Get()
        {
            static InputManager s_Instance;
            return s_Instance;
        }

        ~InputManager() = default;
        InputManager(const InputManager& other) = delete;
        InputManager(InputManager&& other) noexcept = delete;
        InputManager& operator=(const InputManager& other) = delete;
        InputManager& operator=(InputManager&& other) noexcept = delete;

        void Init();
        void Update();
        void Shutdown();
    
        bool IsKeyDown(KeyCode keycode);
        bool IsMouseButtonDown(MouseButton button);
        bool HasKeyPressed(KeyCode keycode);
        bool HasMouseButtonPressed(MouseButton button);
        bool HasHotKeyTriggered(KeyCode triggerKey, std::initializer_list<KeyCode> holdKeys);
        float GetMouseX();
        float GetMouseY();
        std::pair<float, float> GetMousePosition();

        void SetCursorMode(CursorMode mode);
        CursorMode GetCursorMode();

        // Controllers
        bool IsControllerPresent(int id);
        std::vector<int> GetConnectedControllerIDs();
        const Controller* GetController(int id);
        std::string_view GetControllerName(int id);
        bool IsControllerButtonPressed(int controllerID, int button);
        float GetControllerAxis(int controllerID, int axis);
        uint8_t GetControllerHat(int controllerID, int hat);

        const std::map<int, Controller>& GetControllers() { return m_Controllers; }

    private:
        InputManager() = default;

        std::map<int, Controller> m_Controllers;
        std::set<KeyCode> m_PressedKeys;
        std::set<KeyCode> m_PrevPressedKeys;
        std::set<MouseButton> m_PressedButtons;
        std::set<MouseButton> m_PrevPressedButtons;
        float m_PreX = 0, m_PreY = 0;

        EventListener* m_Listener {nullptr};
    };
}
