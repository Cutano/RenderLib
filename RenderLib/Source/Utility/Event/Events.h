#pragma once

#include "Utility/KeyCodes.h"

typedef struct HWND__ *HWND;

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
    };

    struct KeyEvent
    {
        KeyCode Key;
    };

    struct MouseButtonEvent
    {
        MouseButton Button;
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

    struct MouseMovedEvent : Position2DEvent, WindowEvent
    {
        
    };

    struct MouseScrolledEvent : Position2DEvent
    {
        
    };

    struct MouseButtonPressedEvent : MouseButtonEvent
    {
        
    };

    struct MouseButtonReleasedEvent : MouseButtonEvent
    {
        
    };
}
