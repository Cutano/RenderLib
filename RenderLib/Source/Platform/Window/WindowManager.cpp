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

    void WindowManager::Shutdown()
    {
        for (auto& [handle, window] : m_WindowRegistry)
        {
            window.reset();
        }
        
        m_WindowRegistry.clear();
        m_MainWindowHwnd = nullptr;
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

    std::shared_ptr<Window> WindowManager::CreateFromRawGlfwWindow(GLFWwindow* glfwWindow)
    {
        auto window = std::make_shared<Window>(glfwWindow);

        m_WindowRegistry[window->GetHwnd()] = window;
        return window;
    }

    void WindowManager::DestroyWindow(std::shared_ptr<Window>& window)
    {
        m_WindowRegistry.erase(window->GetHwnd());
        window.reset();
    }

    void WindowManager::DestroyWindow(const HWND hwnd)
    {
        m_WindowRegistry[hwnd].reset();
        m_WindowRegistry.erase(hwnd);
    }

    std::shared_ptr<Window> WindowManager::GetWindow(HWND hwnd)
    {
        return m_WindowRegistry[hwnd];
    }
}
