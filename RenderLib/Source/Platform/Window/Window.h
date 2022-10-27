#pragma once

#include <string>
#include <memory>
#include <cstdint>

#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>

namespace Diligent
{
    struct ISwapChain;
}

namespace RL
{
    class EventListener;
    
    class Window
    {
    public:
        Window(std::string title, int width, int height, bool decorated);
        
        void Resize(int width, int height);
        void SetPosition(int x, int y);
        void Update();
        void Present();

        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;
        [[nodiscard]] bool IsMinimized() const;
        [[nodiscard]] HWND GetHwnd() const;
        [[nodiscard]] GLFWwindow* GetGlfWWindow() const;
        [[nodiscard]] std::shared_ptr<Diligent::ISwapChain> GetSwapChain() const;
        void SetWidth(int width);
        void SetHeight(int height);
        void SetSwapChain(const std::shared_ptr<Diligent::ISwapChain>& swapChain);
        void SetSwapChain(Diligent::ISwapChain* swapChain);

    private:
        std::string m_Title;

        int m_Width {1440};
        int m_Height {900};
        bool m_IsMinimized {false};
        bool m_IsVSync {true};

        HWND m_Hwnd {};
        GLFWwindow* m_Window {nullptr};

        std::shared_ptr<EventListener> m_Listener;
        std::shared_ptr<Diligent::ISwapChain> m_SwapChain;
    };
}
