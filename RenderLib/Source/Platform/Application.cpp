#include "Base.h"
#include "Application.h"
#include "Workspace/Workspace.h"
#include "Function/Graphics/GraphicEngine.h"
#include "Function/Scripting/ScriptingEngine.h"
#include "Platform/Window/Window.h"
#include "Platform/Window/WindowManager.h"
#include "Utility/Event/Events.h"
#include "Utility/Event/EventBus.h"
#include "Utility/Event/EventListener.h"
#include "Utility/Timer/Timer.h"
#include "Function/Graphics/ImGui/GuiSystem.h"

#include <filesystem>

namespace RL
{
    Application::Application(const std::vector<std::string>& args)
    {
        if (!args.empty())
        {
            m_StartupParam.ExePath = args.at(0);

            if (args.size() >= 2)
            {
                m_StartupParam.WorkspaceDir = args.at(1);
                Log::Logger()->info("Workspace: {}", m_StartupParam.WorkspaceDir);
            }
            else
            {
                const std::filesystem::path path {m_StartupParam.ExePath};
                const std::filesystem::path defaultWorkspace {"DefaultWorkspace"};
                
                // Create default workspace directory if doesn't exist
                if (!exists(defaultWorkspace)) create_directories(defaultWorkspace);
                
                m_StartupParam.WorkspaceDir = (path.parent_path() / defaultWorkspace).string();
                Log::Logger()->warn("Workspace not specified, using default dir: {}", m_StartupParam.WorkspaceDir);
            }
        }

        InitTimer();
        InitEventBus();
        SetupWorkspace();
        InitScriptingEngine();
        InitGraphicsEngine();
        LoadAsset();
        Preprocess();
        InitMainWindow();
        InitGuiSystem();
    }

    Application::~Application()
    {
        
    }

    void Application::Run()
    {
        while (!m_ShouldExit)
        {
            Timer::Get().Update();
            WindowManager::Get().Update();
            EventBus::Get().Update();
            ScriptingEngine::Get().Update();
            GraphicEngine::Get().Update();
            GuiSystem::Get().Update();

            ScriptingEngine::Get().Render();
            GraphicEngine::Get().Render();
            WindowManager::Get().Present();
        }

        BeforeClose();
    }

    void Application::OnAppWindowClose()
    {
        m_ShouldExit = true;
    }

    void Application::InitTimer()
    {
        Timer::Get().Init();
    }

    void Application::InitEventBus()
    {
        Log::Logger()->info("Initiating event bus...");
        EventBus::Get().Init();

        m_Listener = new EventListener();
        m_Listener->SubscribeEvent<AppWindowCloseEvent>([this](const AppWindowCloseEvent e)
        {
            if (e.Hwnd == WindowManager::Get().GetMainWindow()->GetHwnd())
            {
                OnAppWindowClose();
            }
        });
    }

    void Application::SetupWorkspace()
    {
        Log::Logger()->info("Initiating workspace...");
        Workspace::Get().Init(m_StartupParam.ExePath, m_StartupParam.WorkspaceDir);
    }

    void Application::InitScriptingEngine()
    {
        Log::Logger()->info("Initiating scripting engine...");
        ScriptingEngine::Get().Init();
    }

    void Application::InitGraphicsEngine()
    {
        Log::Logger()->info("Initiating graphics engine...");
        GraphicEngine::Get().Init();
    }

    void Application::LoadAsset()
    {
        Log::Logger()->info("Loading assets...");
    }

    void Application::Preprocess()
    {
        Log::Logger()->info("Preprocessing...");
    }

    void Application::InitMainWindow()
    {
        Log::Logger()->info("Initiating main window...");
        WindowManager::Get().Init();
        WindowManager::Get().ShowMainWindow("Render Lib");

        GraphicEngine::Get().AttachMainWindow();
    }

    void Application::InitGuiSystem()
    {
        GuiSystem::Get().Init();
        GraphicEngine::Get().AttachGuiBackend();
    }

    void Application::BeforeClose()
    {
        delete m_Listener;
        
        WindowManager::Get().Shutdown();
        ScriptingEngine::Get().Shutdown();
        GraphicEngine::Get().Shutdown();
        EventBus::Get().Shutdown();
    }
}
