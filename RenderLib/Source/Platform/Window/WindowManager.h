#pragma once

#include <string>
#include <memory>
#include <map>

#include "Base.h"

typedef struct GLFWwindow GLFWwindow;

namespace RL
{
    class Window;
    
    class WindowManager
    {
    public:
        static WindowManager& Get()
        {
            static WindowManager s_Instance;
            return s_Instance;
        }

        ~WindowManager() = default;
        WindowManager(const WindowManager& other) = delete;
        WindowManager(WindowManager&& other) noexcept = delete;
        WindowManager& operator=(const WindowManager& other) = delete;
        WindowManager& operator=(WindowManager&& other) noexcept = delete;

        void Init();
        void Update();
        void Present();

        void ShowMainWindow(const std::string& title);
        std::shared_ptr<Window> GetMainWindow();

        std::shared_ptr<Window> CreateFromRawGlfwWindow(GLFWwindow* glfwWindow);
        void DestroyWindow(std::shared_ptr<Window>& window);
        void DestroyWindow(HWND hwnd);
        std::shared_ptr<Window> GetWindow(HWND hwnd);

    private:
        WindowManager() = default;

        HWND m_MainWindowHwnd {};
        
        std::map<HWND, std::shared_ptr<Window>> m_WindowRegistry;
    };
    
}
