#pragma once

#include <string>
#include <memory>
#include <cstdint>

#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>

#include <RefCntAutoPtr.hpp>
#include <SwapChain.h>

namespace RL
{
    class EventListener;
    
    class Window
    {
    public:
        Window(std::string title, int width, int height, bool decorated);
        explicit Window(GLFWwindow* glfwWindow);
        
        void Resize(int width, int height);
        void SetPosition(int x, int y);
        void Update();
        void Present();

        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;
        [[nodiscard]] bool IsMinimized() const;
        [[nodiscard]] HWND GetHwnd() const;
        [[nodiscard]] GLFWwindow* GetGlfWWindow() const;
        [[nodiscard]] Diligent::RefCntAutoPtr<Diligent::ISwapChain> GetSwapChain() const;
        void SetWidth(int width);
        void SetHeight(int height);
        void SetSwapChain(const Diligent::RefCntAutoPtr<Diligent::ISwapChain>& swapChain);

    private:
        void RegisterCallbacks();
        std::string m_Title;

        int m_Width {1440};
        int m_Height {900};
        bool m_IsMinimized {false};
        bool m_IsVSync {true};

        HWND m_Hwnd {};
        GLFWwindow* m_Window {nullptr};

        std::shared_ptr<EventListener> m_Listener;
        Diligent::RefCntAutoPtr<Diligent::ISwapChain> m_SwapChain;
    };
}
