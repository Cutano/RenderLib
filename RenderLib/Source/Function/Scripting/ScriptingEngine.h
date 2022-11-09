#pragma once

namespace RL
{
    namespace Interop
    {
        class InteropService;
    }

    class EventListener;
    
    class ScriptingEngine
    {
    public:
        static ScriptingEngine& Get()
        {
            static ScriptingEngine s_Instance;
            return s_Instance;
        }

        void Init();
        void Shutdown();
        void Update();
        void Render();

        ~ScriptingEngine() = default;
        ScriptingEngine(const ScriptingEngine& other) = delete;
        ScriptingEngine(ScriptingEngine&& other) noexcept = delete;
        ScriptingEngine& operator=(const ScriptingEngine& other) = delete;
        ScriptingEngine& operator=(ScriptingEngine&& other) noexcept = delete;

        void BuildAssemblies();
        void LoadAssemblies();
        void UnloadAssemblies();
        void BuildAndLoadAssemblies();

    private:
        ScriptingEngine() = default;

        Interop::InteropService* m_InteropService {nullptr};
        EventListener* m_Listener {nullptr};
    };
}
