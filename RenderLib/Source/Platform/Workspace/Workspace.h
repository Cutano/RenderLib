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

        void Init(const std::string& dir);

    private:
        Workspace() = default;
        
        std::string m_WorkspaceDir;
        std::unique_ptr<FileWatcher> m_Watcher;
    };
}
