#include "Base.h"
#include "Workspace.h"
#include "FileWatcher.h"

#include <filesystem>
#include <chrono>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace RL
{
    void Workspace::Init(const std::string& appPath, const std::string& workDir)
    {
        m_WorkspaceDir = workDir;
        m_ApplicationPath = appPath;

        const std::filesystem::path renderLibProjPath = std::filesystem::path{m_WorkspaceDir} / "RenderLib.json";
        const std::filesystem::path assetDir = std::filesystem::path{m_WorkspaceDir} / "Asset";
        const std::filesystem::path libDir = std::filesystem::path{m_WorkspaceDir} / "Library";
        
        if (!exists(renderLibProjPath))
        {
            InitWorkspace();
        }

        m_Watcher = std::make_unique<FileWatcher>(m_WorkspaceDir);
    }

    std::string& Workspace::GetWorkspaceDir()
    {
        return m_WorkspaceDir;
    }

    std::string& Workspace::GetApplicationPath()
    {
        return m_ApplicationPath;
    }

    void Workspace::InitWorkspace()
    {
        const std::filesystem::path renderLibProjPath = std::filesystem::path{m_WorkspaceDir} / "RenderLib.json";
        const std::filesystem::path assetDir = std::filesystem::path{m_WorkspaceDir} / "Asset";
        const std::filesystem::path libDir = std::filesystem::path{m_WorkspaceDir} / "Library";
        
        const auto nowTime = std::chrono::system_clock::now();
        int64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(nowTime.time_since_epoch()).count();

        const json config = {
            {"projectName", "DefaultWorkspace"},
            {"createdTime", timestamp},
            {"modifiedTime", timestamp}
        };

        auto jsonStr = config.dump(4);

        std::ofstream configFile;
        configFile.open(renderLibProjPath, std::ios::out | std::ios::trunc);
        configFile << jsonStr << std::endl;
        configFile.close();

        create_directories(assetDir);
        create_directories(libDir);
    }
}
