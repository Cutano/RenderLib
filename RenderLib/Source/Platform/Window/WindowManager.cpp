#include "WindowManager.h"
#include "Window.h"

namespace RL
{
    void WindowManager::Init()
    {
        
    }

    void WindowManager::Update()
    {
        for (auto [hwnd, window] : m_WindowRegistry)
        {
            window->Update();
        }
    }

    void WindowManager::ShowMainWindow(const std::string& title)
    {
        const auto window = std::make_shared<Window>(title, 1440, 900, true);

        m_WindowRegistry[window->GetHwnd()] = window;
    }
}
