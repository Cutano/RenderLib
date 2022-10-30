#include "Base.h"
#include "Workspace.h"
#include "FileWatcher.h"

namespace RL
{
    void Workspace::Init(const std::string& appPath, const std::string& workDir)
    {
        m_WorkspaceDir = workDir;
        m_ApplicationPath = appPath;
        m_Watcher = std::make_unique<FileWatcher>(m_WorkspaceDir);
    }

    std::string Workspace::GetWorkspaceDir()
    {
        return m_WorkspaceDir;
    }

    std::string Workspace::GetApplicationPath()
    {
        return m_ApplicationPath;
    }
}
