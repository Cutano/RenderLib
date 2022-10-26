#include "Base.h"
#include "Application.h"

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
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
    }
}
