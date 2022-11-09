#include "Base.h"
#include "ScriptingEngine.h"
#include "Platform/Workspace/Workspace.h"
#include "Platform/Input/InputManager.h"
#include "Interop/InteropService.h"
#include "Utility/Event/Events.h"
#include "Utility/Event/EventListener.h"
#include "Utility/Timer/Timer.h"

#include <filesystem>

namespace RL
{
    const std::wstring g_HotKeyBuildAndLoad {L"BuildAndLoadAssemblies"};
    
    void ScriptingEngine::Init()
    {
        m_Listener = new EventListener();
        m_InteropService = new Interop::InteropService();
        m_InteropService->Init();

        InputManager::Get().RegisterHotKey({
            g_HotKeyBuildAndLoad,
            "Ctrl+R",
            KeyCode::R,
            {KeyCode::LeftControl}
        });

        m_Listener->SubscribeEvent<HotkeyEvent>([this](const HotkeyEvent& e)
        {
            if (e.Name == g_HotKeyBuildAndLoad)
            {
                BuildAndLoadAssemblies();
            }
        });
    }

    void ScriptingEngine::Shutdown()
    {
        m_InteropService->Shutdown();
        delete m_InteropService;
        delete m_Listener;
    }

    void ScriptingEngine::Update()
    {
        m_InteropService->Update(Timer::Get().GetDeltaTime());
    }

    void ScriptingEngine::Render()
    {
        m_InteropService->Render();
    }

    void ScriptingEngine::BuildAssemblies()
    {
        m_InteropService->BuildAssemblies();
    }

    void ScriptingEngine::LoadAssemblies()
    {
        m_InteropService->LoadAssemblies();
    }

    void ScriptingEngine::UnloadAssemblies()
    {
        m_InteropService->UnloadAssemblies();
    }

    void ScriptingEngine::BuildAndLoadAssemblies()
    {
        m_InteropService->BuildAndLoadAssemblies();
    }
}
