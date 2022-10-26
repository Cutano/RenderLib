#include "Base.h"
#include "FileWatcher.h"

namespace RL
{
    FileWatcher::FileWatcher(const std::string& workspaceDir)
    {
        // Create the file system watcher instance
        // efsw::FileWatcher allow a first boolean parameter that indicates if it should start with the generic file watcher instead of the platform specific backend
        m_Watcher = std::make_unique<efsw::FileWatcher>();

        // Add a folder to watch, and get the efsw::WatchID
        // It will watch the /tmp folder recursively ( the third parameter indicates that is recursive )
        // Reporting the files and directories changes to the instance of the listener
        m_WatchId = m_Watcher->addWatch(workspaceDir, this, true);

        // Start watching asynchronously the directories
        m_Watcher->watch();
        Log::Logger()->info("FileWatcher started.");
    }

    FileWatcher::~FileWatcher()
    {
        m_Watcher->removeWatch(m_WatchId);
    }

    void FileWatcher::handleFileAction(efsw::WatchID watchId, const std::string& dir, const std::string& filename,
                                       const efsw::Action action, std::string oldFilename)
    {
        switch (action)
        {
        case efsw::Actions::Add:
            Log::Logger()->trace("File {} added in {}.", filename, dir);
            break;
        case efsw::Actions::Delete:
            Log::Logger()->trace("File {} deleted in {}.", filename, dir);
            break;
        case efsw::Actions::Modified:
            Log::Logger()->trace("File {} modified in {}.", filename, dir);
            break;
        case efsw::Actions::Moved:
            Log::Logger()->trace("File {} moved in {}.", filename, dir);
            break;
        }
    }
}
