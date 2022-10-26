#pragma once

#include <vector>
#include <string>

namespace RL
{
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
        StartupParam m_StartupParam;
    };
}
