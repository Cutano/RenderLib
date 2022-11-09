#pragma once

#include "Utility/KeyCodes.h"

#include <utility>
#include <vector>
#include <string>
#include <unordered_map>
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

    struct HotKey
    {
        std::wstring Name;
        std::string KeyHint;
        KeyCode TriggerKey = static_cast<KeyCode>(0);
        std::set<KeyCode> HoldKeys;
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
        bool IsKeyAllDown(const std::set<KeyCode>& keyCodes);
        bool IsKeyAllDown(std::initializer_list<KeyCode> keyCodes);
        bool IsMouseButtonDown(MouseButton button);
        bool HasKeyPressed(KeyCode keycode) const;
        bool HasMouseButtonPressed(MouseButton button) const;
        bool HasHotKeyTriggered(const HotKey& hotKey);
        float GetMouseX();
        float GetMouseY();
        std::pair<float, float> GetMousePosition();

        void SetCursorMode(CursorMode mode);
        CursorMode GetCursorMode();

        [[nodiscard]] HotKey& GetHotKey(const std::wstring& name);
        [[nodiscard]] bool IsHotKeyRegistered(const HotKey& hotKey) const;
        void RegisterHotKey(const HotKey& hotKey);
        void UnregisterHotKey(const HotKey& hotKey);
        void UnregisterAllHotKey();

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
        std::unordered_map<std::wstring, HotKey> m_HotKeyRegistry;
        float m_PreX = 0, m_PreY = 0;

        EventListener* m_Listener {nullptr};
    };
}
