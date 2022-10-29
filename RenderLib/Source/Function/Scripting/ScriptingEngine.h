#pragma once

namespace RL
{
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

        ~ScriptingEngine() = default;
        ScriptingEngine(const ScriptingEngine& other) = delete;
        ScriptingEngine(ScriptingEngine&& other) noexcept = delete;
        ScriptingEngine& operator=(const ScriptingEngine& other) = delete;
        ScriptingEngine& operator=(ScriptingEngine&& other) noexcept = delete;

    private:
        ScriptingEngine() = default;
    };
}
