#pragma once

#include "Utility/KeyCodes.h"

typedef struct HWND__ *HWND;
typedef struct GLFWmonitor GLFWmonitor;
typedef struct GLFWwindow GLFWwindow;

namespace RL
{
    struct ResizeEvent
    {
        float Width {0};
        float Height {0};
    };

    struct WindowEvent
    {
        HWND Hwnd {};
        GLFWwindow* GlfWWindow {nullptr};
    };

    struct KeyEvent : WindowEvent
    {
        KeyCode Key;
        int ScanCode;
        int Action;
        int Mods;
    };

    struct MouseEvent : WindowEvent
    {
        
    };

    struct MouseButtonEvent : MouseEvent
    {
        MouseButton Button;
        int Action;
        int Mods;
    };

    struct Position2DEvent
    {
        double X {0};
        double Y {0};
    };

    struct Position3DEvent
    {
        double X {0};
        double Y {0};
        double Z {0};
    };

    struct SceneViewportResizeEvent : ResizeEvent
    {
        
    };

    struct AppWindowResizeEvent : ResizeEvent, WindowEvent
    {
        
    };

    struct AppWindowMinimizeEvent : WindowEvent
    {
        bool Minimized {false};
    };

    struct AppWindowCloseEvent : WindowEvent
    {
        
    };

    struct AppWindowFocusEvent : WindowEvent
    {
        bool Focused {true};
    };

    struct KeyPressedEvent : KeyEvent
    {
        
    };

    struct KeyReleasedEvent : KeyEvent
    {
        
    };

    struct KeyRepeatedEvent : KeyEvent
    {
        
    };
    
    struct KeyTypedEvent : KeyEvent
    {
        
    };

    struct MouseMovedEvent : Position2DEvent, MouseEvent
    {
        
    };

    struct MouseEnteredEvent : MouseEvent
    {
        bool Entered {true};
    };

    struct MouseScrolledEvent : Position2DEvent, MouseEvent
    {
        
    };

    struct MouseButtonPressedEvent : MouseButtonEvent
    {
        
    };

    struct MouseButtonReleasedEvent : MouseButtonEvent
    {
        
    };

    struct MonitorEvent
    {
        GLFWmonitor* Monitor {nullptr};
        int Event {0};
    };
}
