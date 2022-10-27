#pragma once

#include <string>
#include <memory>
#include <map>

#include "Base.h"

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

        void ShowMainWindow(const std::string& title);
        std::shared_ptr<Window> GetMainWindow();

    private:
        WindowManager() = default;

        HWND m_MainWindowHwnd {};
        
        std::map<HWND, std::shared_ptr<Window>> m_WindowRegistry;
    };
    
}
