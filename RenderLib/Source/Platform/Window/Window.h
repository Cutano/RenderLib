#pragma once

#include <string>
#include <cstdint>

#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>

namespace RL
{
    class Window
    {
    public:
        Window(std::string title, int width, int height, bool decorated);
        
        void Resize(int width, int height);
        void SetPosition(int x, int y);
        void Update();

        [[nodiscard]] uint32_t GetWidth() const;
        [[nodiscard]] uint32_t GetHeight() const;
        [[nodiscard]] HWND GetHwnd() const;
        void SetWidth(uint32_t width);
        void SetHeight(uint32_t height);

    private:
        std::string m_Title;

        int m_Width {1440};
        int m_Height {900};

        HWND m_Hwnd {};
        
        GLFWwindow* m_Window {nullptr};
    };
}
