#pragma once

#include <string>
#include <memory>

#include <efsw/efsw.hpp>

namespace RL
{
    class FileWatcher final : public efsw::FileWatchListener
    {
    public:
        explicit FileWatcher(const std::string& workspaceDir);
        ~FileWatcher() override;

        FileWatcher(const FileWatcher& other) = delete;
        FileWatcher(FileWatcher&& other) noexcept = delete;
        FileWatcher& operator=(const FileWatcher& other) = delete;
        FileWatcher& operator=(FileWatcher&& other) noexcept = delete;

        void handleFileAction(efsw::WatchID watchId, const std::string& dir, const std::string& filename,
                              efsw::Action action, std::string oldFilename) override;

    private:
        std::unique_ptr<efsw::FileWatcher> m_Watcher;
        efsw::WatchID m_WatchId;
    };
}
