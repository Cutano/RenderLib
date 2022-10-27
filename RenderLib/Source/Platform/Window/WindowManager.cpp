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

    void WindowManager::Present()
    {
        for (auto [hwnd, window] : m_WindowRegistry)
        {
            window->Present();
        }
    }

    void WindowManager::ShowMainWindow(const std::string& title)
    {
        const auto window = std::make_shared<Window>(title, 1440, 900, true);

        m_WindowRegistry[window->GetHwnd()] = window;
        m_MainWindowHwnd = window->GetHwnd();
    }

    std::shared_ptr<Window> WindowManager::GetMainWindow()
    {
        return m_WindowRegistry[m_MainWindowHwnd];
    }
}
