﻿#include "Base.h"
#include "ScriptingEngine.h"
#include "Platform/Workspace/Workspace.h"
#include "Interop/InteropService.h"
#include "Utility/Timer/Timer.h"

#include <filesystem>

namespace RL
{
    void ScriptingEngine::Init()
    {
        m_InteropService = new Interop::InteropService();
        m_InteropService->Init();
    }

    void ScriptingEngine::Shutdown()
    {
        m_InteropService->Shutdown();
        delete m_InteropService;
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
