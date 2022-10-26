#pragma once

#include <string>
#include <memory>

namespace RL
{
    class FileWatcher;
    
    class Workspace
    {
    public:
        explicit Workspace(std::string dir);

    private:
        std::string m_WorkspaceDir;
        std::unique_ptr<FileWatcher> m_Watcher;
    };
}
