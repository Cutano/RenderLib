#pragma once

#include <string>
#include <memory>

#include "FileWatcher.h"

namespace RL
{
    class FileWatcher;
    
    class Workspace
    {
    public:
        static Workspace& Get()
        {
            static Workspace s_Instance;
            return s_Instance;
        }
        
        ~Workspace() = default;
        Workspace(const Workspace& other) = delete;
        Workspace(Workspace&& other) noexcept = delete;
        Workspace& operator=(const Workspace& other) = delete;
        Workspace& operator=(Workspace&& other) noexcept = delete;

        void Init(const std::string& appPath, const std::string& workDir);
        
        std::string& GetWorkspaceDir();
        std::string& GetApplicationPath();

    private:
        Workspace() = default;

        void InitWorkspace();
        
        std::string m_WorkspaceDir;
        std::string m_ApplicationPath;
        std::unique_ptr<FileWatcher> m_Watcher;
    };
}
