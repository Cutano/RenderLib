#pragma once

#include "Utility/KeyCodes.h"

namespace RL
{
    struct ResizeEvent
    {
        float Width {0};
        float Height {0};
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
        float X;
        float Y;
    };

    struct Position3DEvent
    {
        float X;
        float Y;
        float Z;
    };

    struct SceneWindowResizeEvent : ResizeEvent
    {
        
    };

    struct AppWindowResizeEvent : ResizeEvent
    {
        
    };

    struct AppWindowMinimizeEvent
    {
        
    };

    struct AppWindowCloseEvent
    {
        
    };

    struct KeyPressedEvent : KeyEvent
    {
        
    };

    struct KeyReleasedEvent : KeyEvent
    {
        
    };
    
    struct KeyTypedEvent : KeyEvent
    {
        
    };

    struct MouseMovedEvent : Position2DEvent
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
