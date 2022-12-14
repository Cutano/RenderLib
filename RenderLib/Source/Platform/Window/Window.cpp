#include "Base.h"
#include "Window.h"
#include "Utility/Event/Events.h"
#include "Utility/Event/EventBus.h"
#include "Utility/Event/EventListener.h"

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

        m_Window = glfwCreateWindow(width, height, m_Title.c_str(), nullptr, nullptr);
        m_Hwnd = glfwGetWin32Window(m_Window);

        glfwSetWindowUserPointer(m_Window, &m_Hwnd);

        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        else
            Log::Logger()->warn("Platform", "Raw mouse motion not supported.");

    	// Update window size to actual size
    	{
        	int _width, _height;
        	glfwGetWindowSize(m_Window, &_width, &_height);
        	m_Width = _width;
        	m_Height = _height;
    	}

    	RegisterCallbacks();
    }

	Window::Window(GLFWwindow* glfwWindow)
    {
    	m_Window = glfwWindow;
    	m_Hwnd = glfwGetWin32Window(m_Window);

    	glfwSetWindowUserPointer(m_Window, &m_Hwnd);

    	if (glfwRawMouseMotionSupported())
    		glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    	else
    		Log::Logger()->warn("Platform", "Raw mouse motion not supported.");

    	// Update window size to actual size
    	{
    		int _width, _height;
    		glfwGetWindowSize(m_Window, &_width, &_height);
    		m_Width = _width;
    		m_Height = _height;
    	}

    	RegisterCallbacks();
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
    	glfwPollEvents();
    }

    void Window::Present()
    {
    	if (m_SwapChain != nullptr)
    	{
    		m_SwapChain->Present();
    	}
    }

    int Window::GetWidth() const
    {
        return m_Width;
    }

    void Window::SetWidth(const int width)
    {
        m_Width = width;
    }

    int Window::GetHeight() const
    {
        return m_Height;
    }

    bool Window::IsMinimized() const
    {
    	return m_IsMinimized;
    }

    HWND Window::GetHwnd() const
    {
        return m_Hwnd;
    }

    GLFWwindow* Window::GetGlfWWindow() const
    {
    	return m_Window;
    }

    Diligent::RefCntAutoPtr<Diligent::ISwapChain> Window::GetSwapChain() const
    {
    	return m_SwapChain;
    }

    void Window::SetHeight(const int height)
    {
        m_Height = height;
    }

    void Window::SetSwapChain(const Diligent::RefCntAutoPtr<Diligent::ISwapChain>& swapChain)
    {
	    m_SwapChain = swapChain;
    }

    void Window::RegisterCallbacks()
    {
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
	        const auto& hwnd = *static_cast<HWND*>(glfwGetWindowUserPointer(window));
        	
	        AppWindowResizeEvent e;
        	e.GlfWWindow = window;
	        e.Hwnd = hwnd;
	        e.Width = static_cast<float>(width);
	        e.Height = static_cast<float>(height);
        	
	        EventBus::Get().SpreadEvent(e);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
	        const auto& hwnd = *static_cast<HWND*>(glfwGetWindowUserPointer(window));
        	
	        AppWindowCloseEvent e;
        	e.GlfWWindow = window;
	        e.Hwnd = hwnd;

	        EventBus::Get().SpreadEvent(e);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
        	const auto& hwnd = *static_cast<HWND*>(glfwGetWindowUserPointer(window));
        	
	        switch (action)
	        {
	        case GLFW_PRESS:
		        {
	        		KeyPressedEvent e;
	        		e.GlfWWindow = window;
	        		e.Hwnd = hwnd;
	        		e.Action = action;
	        		e.ScanCode = scancode;
	        		e.Mods = mods;
			        e.Key = static_cast<KeyCode>(key);
			        EventBus::Get().SpreadEvent(e);
			        break;
		        }
	        case GLFW_RELEASE:
		        {
	        		KeyReleasedEvent e;
	        		e.GlfWWindow = window;
					e.Hwnd = hwnd;
	        		e.Action = action;
					e.ScanCode = scancode;
					e.Mods = mods;
			        e.Key = static_cast<KeyCode>(key);
			        EventBus::Get().SpreadEvent(e);
			        break;
		        }
	        case GLFW_REPEAT:
		        {
	        		KeyRepeatedEvent e;
	        		e.GlfWWindow = window;
					e.Hwnd = hwnd;
	        		e.Action = action;
					e.ScanCode = scancode;
					e.Mods = mods;
			        e.Key = static_cast<KeyCode>(key);
			        EventBus::Get().SpreadEvent(e);
			        break;
		        }
	        }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t codepoint)
        {
        	const auto& hwnd = *static_cast<HWND*>(glfwGetWindowUserPointer(window));
        	
	        KeyTypedEvent e;
        	e.GlfWWindow = window;
        	e.Hwnd = hwnd;
	        e.Key = static_cast<KeyCode>(codepoint);
	        EventBus::Get().SpreadEvent(e);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
        	const auto& hwnd = *static_cast<HWND*>(glfwGetWindowUserPointer(window));
        	
	        switch (action)
	        {
	        case GLFW_PRESS:
		        {
	        		MouseButtonPressedEvent e;
	        		e.GlfWWindow = window;
					e.Hwnd = hwnd;
	        		e.Action = action;
	        		e.Mods = mods;
			        e.Button = static_cast<MouseButton>(button);
			        EventBus::Get().SpreadEvent(e);
			        break;
		        }
	        case GLFW_RELEASE:
		        {
	        		MouseButtonReleasedEvent e;
	        		e.GlfWWindow = window;
					e.Hwnd = hwnd;
	        		e.Action = action;
					e.Mods = mods;
			        e.Button = static_cast<MouseButton>(button);
			        EventBus::Get().SpreadEvent(e);
			        break;
		        }
	        }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
        	const auto& hwnd = *static_cast<HWND*>(glfwGetWindowUserPointer(window));
        	
	        MouseScrolledEvent e;
        	e.GlfWWindow = window;
        	e.Hwnd = hwnd;
	        e.X = xOffset;
	        e.Y = yOffset;
        	
	        EventBus::Get().SpreadEvent(e);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
        {
	        const auto& hwnd = *static_cast<HWND*>(glfwGetWindowUserPointer(window));

	        MouseMovedEvent e;
        	e.GlfWWindow = window;
	        e.Hwnd = hwnd;
	        e.X = x;
	        e.Y = y;

	        EventBus::Get().SpreadEvent(e);
        });

		glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* window, int iconified)
		{
			const auto& hwnd = *static_cast<HWND*>(glfwGetWindowUserPointer(window));
			
			AppWindowMinimizeEvent e;
			e.GlfWWindow = window;
			e.Hwnd = hwnd;
			e.Minimized = static_cast<bool>(iconified);
			
			EventBus::Get().SpreadEvent(e);
		});

    	glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
    	{
    		const auto& hwnd = *static_cast<HWND*>(glfwGetWindowUserPointer(window));

    		AppWindowFocusEvent e;
    		e.GlfWWindow = window;
    		e.Hwnd = hwnd;
    		e.Focused = static_cast<bool>(focused);

    		EventBus::Get().SpreadEvent(e);
    	});

    	glfwSetCursorEnterCallback(m_Window, [](GLFWwindow* window, int entered)
    	{
    		const auto& hwnd = *static_cast<HWND*>(glfwGetWindowUserPointer(window));

			MouseEnteredEvent e;
    		e.GlfWWindow = window;
			e.Hwnd = hwnd;
			e.Entered = static_cast<bool>(entered);

			EventBus::Get().SpreadEvent(e);
    	});

    	glfwSetMonitorCallback([](GLFWmonitor* monitor, int event)
    	{
    		MonitorEvent e;
    		e.Monitor = monitor;
    		e.Event = event;

    		EventBus::Get().SpreadEvent(e);
    	});

    	m_Listener = std::make_shared<EventListener>();

    	m_Listener->SubscribeEvent<AppWindowResizeEvent>([this](const AppWindowResizeEvent e)
		{
			if (e.Hwnd == m_Hwnd)
			{
				m_Width = static_cast<int>(e.Width);
				m_Height = static_cast<int>(e.Height);

				if (m_SwapChain != nullptr)
				{
					m_SwapChain->Resize(m_Width, m_Height);
				}
			}
		});

    	m_Listener->SubscribeEvent<AppWindowMinimizeEvent>([this](const AppWindowMinimizeEvent e)
		{
			if (e.Hwnd == m_Hwnd)
			{
				m_IsMinimized = e.Minimized;
			}
		});
    }
}
