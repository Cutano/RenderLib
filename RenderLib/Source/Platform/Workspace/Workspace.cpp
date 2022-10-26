#include "Base.h"
#include "Workspace.h"
#include "FileWatcher.h"

namespace RL
{
    Workspace::Workspace(std::string dir) : m_WorkspaceDir(std::move(dir))
    {
        m_Watcher = std::make_unique<FileWatcher>(m_WorkspaceDir);
    }
}
