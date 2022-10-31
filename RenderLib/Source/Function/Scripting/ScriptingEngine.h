#pragma once

namespace RL
{
    struct DirParam
    {
        
    };
    
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

        void CsharpFileChanged(int action, const std::wstring& path);
        void RecompileAssembly();
        void ReloadAssembly();

    private:
        ScriptingEngine() = default;

        bool LoadHostFxr();
    };
}
