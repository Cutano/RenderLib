#include "Base.h"
#include "ScriptingEngine.h"
#include "Platform/Workspace/Workspace.h"

#include <nethost.h>
#include <hostfxr.h>
#include <coreclr_delegates.h>

#include <filesystem>

namespace RL
{
    // Globals to hold hostfxr exports
    hostfxr_initialize_for_runtime_config_fn InitRuntimeConfigDelegate;
    hostfxr_get_runtime_delegate_fn GetRuntimeDelegateDelegate;
    hostfxr_set_error_writer_fn SetErrorWriterDelegate;
    hostfxr_close_fn CloseFxrDelegate;

    using string_t = std::basic_string<char_t>;

    struct ManagedFunctionPayload
    {
        void(*Update)(double);
    } managedPayload;

    void* get_export(void* h, const char* name)
    {
        void* f = GetProcAddress(static_cast<HMODULE>(h), name);
        assert(f != nullptr);
        return f;
    }
    
    void ScriptingEngine::Init()
    {
        if (!LoadHostFxr())
        {
            Log::Logger()->error("Cannot load Dotnet hostfxr, check if Dotnet 6 installed!");
        }

        SetErrorWriterDelegate([](const char_t *message)
        {
            Log::Logger()->error("Scripting: {}", ConvertString(message));
        });

        std::filesystem::path rootPath {Workspace::Get().GetApplicationPath()};
        std::filesystem::path rootDir = rootPath.parent_path();

        const string_t config_path = rootDir / "Resource" / "Config" / "RenderLib.runtimeconfig.json";

        // Load .NET Core
        void *loadAssemblyAndGetFunctionPointerVoid = nullptr;
        hostfxr_handle cxt = nullptr;
        int rc = InitRuntimeConfigDelegate(config_path.c_str(), nullptr, &cxt);
        if (rc != 0 || cxt == nullptr)
        {
            Log::Logger()->error("Init failed: {}", rc);
            CloseFxrDelegate(cxt);
            assert(false);
        }

        // Get the load assembly function pointer
        rc = GetRuntimeDelegateDelegate(
            cxt,
            hdt_load_assembly_and_get_function_pointer,
            &loadAssemblyAndGetFunctionPointerVoid);
        if (rc != 0 || loadAssemblyAndGetFunctionPointerVoid == nullptr)
            Log::Logger()->error("Get delegate failed: {}", rc);

        CloseFxrDelegate(cxt);
        const auto loadAssemblyAndGetFunctionPointer =
            static_cast<load_assembly_and_get_function_pointer_fn>(loadAssemblyAndGetFunctionPointerVoid);
        
        RL_ASSERT(loadAssemblyAndGetFunctionPointer != nullptr, "Failure: get_dotnet_load_assembly()")

        const string_t dotnetLibPath = rootDir / "net6.0" / "ScriptingCore.dll";
        const char_t *dotnet_type = L"ScriptingCore.Entry, ScriptingCore";
        typedef ManagedFunctionPayload (CORECLR_DELEGATE_CALLTYPE *custom_entry_point_fn)(const char_t* args);
        custom_entry_point_fn init = nullptr;
        rc = loadAssemblyAndGetFunctionPointer(
            dotnetLibPath.c_str(),
            dotnet_type,
            L"Init" /*method_name*/,
            UNMANAGEDCALLERSONLY_METHOD,
            nullptr,
            reinterpret_cast<void**>(&init));

        RL_ASSERT(rc == 0 && init != nullptr, "Failure: load_assembly_and_get_function_pointer()")

        managedPayload = init(L"Hello 你好");
    }

    void ScriptingEngine::Shutdown()
    {
        
    }

    void ScriptingEngine::Update()
    {
        managedPayload.Update(0);
    }

    bool ScriptingEngine::LoadHostFxr()
    {
        // Pre-allocate a large buffer for the path to hostfxr
        char_t buffer[MAX_PATH];
        size_t bufferSize = sizeof buffer / sizeof(char_t);
        if (const auto rc = get_hostfxr_path(buffer, &bufferSize, nullptr); rc != 0)
            return false;

        // Load hostfxr and get desired exports
        const HMODULE h = LoadLibraryW(buffer);
        assert(h != nullptr);
        void *lib = h;

        InitRuntimeConfigDelegate = static_cast<hostfxr_initialize_for_runtime_config_fn>(get_export(lib, "hostfxr_initialize_for_runtime_config"));
        GetRuntimeDelegateDelegate = static_cast<hostfxr_get_runtime_delegate_fn>(get_export(lib, "hostfxr_get_runtime_delegate"));
        SetErrorWriterDelegate = static_cast<hostfxr_set_error_writer_fn>(get_export(lib, "hostfxr_set_error_writer"));
        CloseFxrDelegate = static_cast<hostfxr_close_fn>(get_export(lib, "hostfxr_close"));

        return InitRuntimeConfigDelegate && GetRuntimeDelegateDelegate && SetErrorWriterDelegate && CloseFxrDelegate;
    }
}
