#include "Base.h"
#include "Window.h"
#include "Utility/Event/Events.h"
#include "Utility/Event/EventBus.h"

namespace RL
{
    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        Log::Logger()->error("GLFW Error ({0}): {1}", error, description);
    }
    
    Window::Window(std::string title, int width, int height, bool decorated = false)
    : m_Title(std::move(title)), m_Width(width), m_Height(height)
    {
        if (!s_GLFWInitialized)
        {
            const int success = glfwInit();
            RL_ASSERT(success, "GLFW init fails!")
            glfwSetErrorCallback(GLFWErrorCallback);

            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_DECORATED, decorated);

        m_Window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), m_Title.c_str(), nullptr, nullptr);
        m_Hwnd = glfwGetWin32Window(m_Window);

        glfwSetWindowUserPointer(m_Window, &m_Hwnd);

        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        else
            Log::Logger()->warn("Platform", "Raw mouse motion not supported.");

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
	        const auto& hwnd = *static_cast<HWND*>(glfwGetWindowUserPointer(window));
        	
	        AppWindowResizeEvent e;
	        e.Hwnd = hwnd;
	        e.Width = static_cast<float>(width);
	        e.Height = static_cast<float>(height);
        	
	        EventBus::Get().SpreadEvent<AppWindowResizeEvent>(e);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
	        const auto& hwnd = *static_cast<HWND*>(glfwGetWindowUserPointer(window));
        	
	        AppWindowCloseEvent e;
	        e.Hwnd = hwnd;

	        EventBus::Get().SpreadEvent<AppWindowCloseEvent>(e);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
	        switch (action)
	        {
	        case GLFW_PRESS:
		        {
	        		KeyPressedEvent e;
			        e.Key = static_cast<KeyCode>(key);
			        EventBus::Get().SpreadEvent<KeyPressedEvent>(e);
			        break;
		        }
	        case GLFW_RELEASE:
		        {
	        		KeyReleasedEvent e;
			        e.Key = static_cast<KeyCode>(key);
			        EventBus::Get().SpreadEvent<KeyReleasedEvent>(e);
			        break;
		        }
	        case GLFW_REPEAT:
		        {
	        		KeyRepeatedEvent e;
			        e.Key = static_cast<KeyCode>(key);
			        EventBus::Get().SpreadEvent<KeyRepeatedEvent>(e);
			        break;
		        }
	        }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t codepoint)
        {
	        KeyTypedEvent e;
	        e.Key = static_cast<KeyCode>(codepoint);
	        EventBus::Get().SpreadEvent<KeyTypedEvent>(e);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
	        switch (action)
	        {
	        case GLFW_PRESS:
		        {
	        		MouseButtonPressedEvent e;
			        e.Button = static_cast<MouseButton>(button);
			        EventBus::Get().SpreadEvent<MouseButtonPressedEvent>(e);
			        break;
		        }
	        case GLFW_RELEASE:
		        {
	        		MouseButtonReleasedEvent e;
			        e.Button = static_cast<MouseButton>(button);
			        EventBus::Get().SpreadEvent<MouseButtonReleasedEvent>(e);
			        break;
		        }
	        }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
	        MouseScrolledEvent e;
	        e.X = xOffset;
	        e.Y = yOffset;
        	
	        EventBus::Get().SpreadEvent<MouseScrolledEvent>(e);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
        {
	        const auto& hwnd = *static_cast<HWND*>(glfwGetWindowUserPointer(window));

	        MouseMovedEvent e;
	        e.Hwnd = hwnd;
	        e.X = x;
	        e.Y = y;

	        EventBus::Get().SpreadEvent<MouseMovedEvent>(e);
        });

		glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* window, int iconified)
		{
			const auto& hwnd = *static_cast<HWND*>(glfwGetWindowUserPointer(window));
			
			AppWindowMinimizeEvent e;
			e.Hwnd = hwnd;
			
			EventBus::Get().SpreadEvent<AppWindowMinimizeEvent>(e);
		});

    	// Update window size to actual size
    	{
        	int _width, _height;
        	glfwGetWindowSize(m_Window, &_width, &_height);
        	m_Width = width;
        	m_Height = height;
    	}
    }

    void Window::Resize(const int width, const int height)
    {
        m_Width = width;
        m_Height = height;

    	glfwSetWindowSize(m_Window, width, height);
    }

    void Window::SetPosition(const int x, const int y)
    {
    	glfwSetWindowPos(m_Window, x, y);
    }

    void Window::Update()
    {
    	glfwWaitEventsTimeout(0.005);
    }

    uint32_t Window::GetWidth() const
    {
        return m_Width;
    }

    void Window::SetWidth(const uint32_t width)
    {
        m_Width = width;
    }

    uint32_t Window::GetHeight() const
    {
        return m_Height;
    }

    HWND Window::GetHwnd() const
    {
        return m_Hwnd;
    }

    void Window::SetHeight(const uint32_t height)
    {
        m_Height = height;
    }
}
