#include "Base.h"
#include "FileWatcher.h"
#include "Utility/Event/EventBus.h"
#include "Utility/Event/Events.h"
#include "Function/Scripting/ScriptingEngine.h"

#include <filesystem>


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
        m_WatchId = m_Watcher->addWatch(workspaceDir + "\\Asset", this, true);

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
        const auto wFilename = std::filesystem::path{ConvertString(filename)};
        const auto wDir = std::filesystem::path{ConvertString(dir)};

        const std::filesystem::path path {wDir / wFilename};

        if (wFilename.has_extension() && wFilename.extension() == L".cs")
        {
            SourceFileChangedEvent e;
            e.Path = path;
            e.Type = static_cast<FileEvent::EventType>(action);
            EventBus::Get().SpreadEvent(e);
        }
        
        switch (action)
        {
        case efsw::Actions::Add:
            Log::Logger()->trace("File {} added in {}.", ConvertString(wFilename), ConvertString(wDir));
            break;
        case efsw::Actions::Delete:
            Log::Logger()->trace("File {} deleted in {}.", ConvertString(wFilename), ConvertString(wDir));
            break;
        case efsw::Actions::Modified:
            Log::Logger()->trace("File {} modified in {}.", ConvertString(wFilename), ConvertString(wDir));
            break;
        case efsw::Actions::Moved:
            Log::Logger()->trace("File {} moved in {}.", ConvertString(wFilename), ConvertString(wDir));
            break;
        }
    }
}
