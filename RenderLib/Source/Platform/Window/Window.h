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
        Window(std::string title, uint32_t width, uint32_t height, bool decorated);

        void Show();
        void Resize(uint32_t width, uint32_t height);
        void Update();

        [[nodiscard]] uint32_t GetWidth() const;
        [[nodiscard]] uint32_t GetHeight() const;
        [[nodiscard]] HWND GetHwnd() const;
        void SetWidth(uint32_t width);
        void SetHeight(uint32_t height);

    private:
        std::string m_Title;

        uint32_t m_Width {1440};
        uint32_t m_Height {900};

        HWND m_Hwnd {};
        
        GLFWwindow* m_Window {nullptr};
    };
}
