#include "Base.h"
#include "Workspace.h"
#include "FileWatcher.h"

namespace RL
{
    void Workspace::Init(const std::string& dir)
    {
        m_WorkspaceDir = dir;
        m_Watcher = std::make_unique<FileWatcher>(m_WorkspaceDir);
    }
}
