#pragma once

#include <vector>
#include <string>
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

    private:
        void InitEventBus();
        void SetupWorkspace();
        void InitGraphicsEngine();
        void LoadAsset();
        void Preprocess();
        void InitMainWindow();
        
        StartupParam m_StartupParam;
    };
}
