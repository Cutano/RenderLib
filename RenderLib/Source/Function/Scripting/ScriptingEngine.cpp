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
    hostfxr_initialize_for_runtime_config_fn init_fptr;
    hostfxr_get_runtime_delegate_fn get_delegate_fptr;
    hostfxr_set_error_writer_fn set_error_writer_fptr;
    hostfxr_close_fn close_fptr;

    using string_t = std::basic_string<char_t>;

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

        set_error_writer_fptr([](const char_t *message)
        {
            Log::Logger()->error("Scripting: {}", ConvertString(message));
        });

        std::filesystem::path rootPath {Workspace::Get().GetApplicationPath()};
        std::filesystem::path rootDir = rootPath.parent_path();

        const string_t config_path = rootDir / "Resource" / "Config" / "RenderLib.runtimeconfig.json";

        // Load .NET Core
        void *load_assembly_and_get_function_pointer_void = nullptr;
        hostfxr_handle cxt = nullptr;
        int rc = init_fptr(config_path.c_str(), nullptr, &cxt);
        if (rc != 0 || cxt == nullptr)
        {
            Log::Logger()->error("Init failed: {}", rc);
            close_fptr(cxt);
            assert(false);
        }

        // Get the load assembly function pointer
        rc = get_delegate_fptr(
            cxt,
            hdt_load_assembly_and_get_function_pointer,
            &load_assembly_and_get_function_pointer_void);
        if (rc != 0 || load_assembly_and_get_function_pointer_void == nullptr)
            Log::Logger()->error("Get delegate failed: {}", rc);

        close_fptr(cxt);
        const auto load_assembly_and_get_function_pointer =
            static_cast<load_assembly_and_get_function_pointer_fn>(load_assembly_and_get_function_pointer_void);
        
        RL_ASSERT(load_assembly_and_get_function_pointer != nullptr, "Failure: get_dotnet_load_assembly()")

        const string_t dotnetlib_path = rootDir / "net6.0" / "ScriptingCore.dll";
        const char_t *dotnet_type = L"ScriptingCore.Entry, ScriptingCore";
        typedef void (CORECLR_DELEGATE_CALLTYPE *custom_entry_point_fn)(const char_t* args);
        custom_entry_point_fn init = nullptr;
        rc = load_assembly_and_get_function_pointer(
            dotnetlib_path.c_str(),
            dotnet_type,
            L"Init" /*method_name*/,
            UNMANAGEDCALLERSONLY_METHOD,
            nullptr,
            (void**)&init);

        RL_ASSERT(rc == 0 && init != nullptr, "Failure: load_assembly_and_get_function_pointer()")

        init(L"Hello 你好");
    }

    void ScriptingEngine::Shutdown()
    {
        
    }

    void ScriptingEngine::Update()
    {
        
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

        init_fptr = static_cast<hostfxr_initialize_for_runtime_config_fn>(get_export(lib, "hostfxr_initialize_for_runtime_config"));
        get_delegate_fptr = static_cast<hostfxr_get_runtime_delegate_fn>(get_export(lib, "hostfxr_get_runtime_delegate"));
        set_error_writer_fptr = static_cast<hostfxr_set_error_writer_fn>(get_export(lib, "hostfxr_set_error_writer"));
        close_fptr = static_cast<hostfxr_close_fn>(get_export(lib, "hostfxr_close"));

        return init_fptr && get_delegate_fptr && set_error_writer_fptr && close_fptr;
    }
}
