#pragma once

#include <vector>
#include <string>
#include <atomic>
#include <memory>

namespace RL
{
    // Application Startup Routine:
    //   Init Event Bus
    //   Setup Workspace
    //   Init Graphics Engine
    //   Load Assets
    //   Perform Preprocess
    //   Init Main Window
    //   Init GUI System

    class EventListener;
    
    struct StartupParam
    {
        std::string ExePath;
        std::string WorkspaceDir;
    };
    
    class Application
    {
    public:
        explicit Application(const std::vector<std::string>& args);
        ~Application();

        Application(const Application& other) = delete;
        Application(Application&& other) noexcept = delete;
        Application& operator=(const Application& other) = delete;
        Application& operator=(Application&& other) noexcept = delete;

        void Run();

        void OnAppWindowClose();

    private:
        void InitEventBus();
        void SetupWorkspace();
        void InitGraphicsEngine();
        void LoadAsset();
        void Preprocess();
        void InitMainWindow();
        void InitGuiSystem();
        
        StartupParam m_StartupParam;

        std::atomic_bool m_ShouldExit {false};

        EventListener* m_Listener {nullptr};
    };
}
