#include "Base.h"
#include "InteropService.h"
#include "Bindings.h"
#include "Platform/Workspace/Workspace.h"
#include "Utility/Event/Events.h"
#include "Utility/Event/EventListener.h"

#include <nethost.h>
#include <hostfxr.h>
#include <coreclr_delegates.h>

#include <filesystem>

namespace RL::Interop
{
    hostfxr_initialize_for_runtime_config_fn InitRuntimeConfigDelegate;
    hostfxr_get_runtime_delegate_fn GetRuntimeDelegateDelegate;
    hostfxr_set_error_writer_fn SetErrorWriterDelegate;
    hostfxr_close_fn CloseFxrDelegate;

    struct ManagedFunctionPayload
    {
        void(*Update)(double);
        void(*Render)();
        void(*OnSourceFileChanged)(int, const wchar_t*);
        void(*BuildAssemblies)();
        void(*LoadAssemblies)();
        void(*UnloadAssemblies)();
        void(*BuildAndLoadAssemblies)();
    } managedPayload;

    struct UnmanagedFunctionPayload
    {
        void* WorkspaceGetAppPath;
        void* WorkspaceGetWorkspaceDir;
    } unmanagedFunctionPayload;

    void* GetExport(void* h, const char* name)
    {
        void* f = GetProcAddress(static_cast<HMODULE>(h), name);
        assert(f != nullptr);
        return f;
    }

    bool LoadHostFxr()
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

        InitRuntimeConfigDelegate = static_cast<hostfxr_initialize_for_runtime_config_fn>(GetExport(lib, "hostfxr_initialize_for_runtime_config"));
        GetRuntimeDelegateDelegate = static_cast<hostfxr_get_runtime_delegate_fn>(GetExport(lib, "hostfxr_get_runtime_delegate"));
        SetErrorWriterDelegate = static_cast<hostfxr_set_error_writer_fn>(GetExport(lib, "hostfxr_set_error_writer"));
        CloseFxrDelegate = static_cast<hostfxr_close_fn>(GetExport(lib, "hostfxr_close"));

        return InitRuntimeConfigDelegate && GetRuntimeDelegateDelegate && SetErrorWriterDelegate && CloseFxrDelegate;
    }
    
    void InteropService::Init()
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

        const std::wstring config_path = rootDir / "ScriptingCore.runtimeconfig.json";

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

        const std::wstring dotnetLibPath = rootDir / "ScriptingCore.dll";
        const char_t *dotnet_type = L"ScriptingCore.Entry, ScriptingCore";

        typedef ManagedFunctionPayload (CORECLR_DELEGATE_CALLTYPE *custom_entry_point_fn)(UnmanagedFunctionPayload args);
        custom_entry_point_fn init = nullptr;
        rc = loadAssemblyAndGetFunctionPointer(
            dotnetLibPath.c_str(),
            dotnet_type,
            L"Init" /*method_name*/,
            UNMANAGEDCALLERSONLY_METHOD,
            nullptr,
            reinterpret_cast<void**>(&init));

        RL_ASSERT(rc == 0 && init != nullptr, "Failure: load_assembly_and_get_function_pointer()")

        if (init)
        {
            unmanagedFunctionPayload.WorkspaceGetAppPath = (void*)&Binding::Workspace::GetAppPath;
            unmanagedFunctionPayload.WorkspaceGetWorkspaceDir = (void*)&Binding::Workspace::GetWorkspaceDir;
            managedPayload = init(unmanagedFunctionPayload);
            
            Log::Logger()->info("Host loaded successfully!");
        }
        else
        {
            Log::Logger()->error("Host runtime assembly unable to load the initialization function!");
        }

        m_Listener = new EventListener();

        m_Listener->SubscribeEvent<SourceFileChangedEvent>([](const SourceFileChangedEvent& e)
        {
            if (managedPayload.OnSourceFileChanged)
            {
                managedPayload.OnSourceFileChanged(e.Type, e.Path.data());
            }
        });
    }

    void InteropService::Shutdown()
    {
        delete m_Listener;
    }

    void InteropService::Update(double dt)
    {
        if (managedPayload.Update)
        {
            managedPayload.Update(dt);
        }
    }

    void InteropService::Render()
    {
        if (managedPayload.Render)
        {
            managedPayload.Render();
        }
    }

    void InteropService::BuildAssemblies()
    {
        if (managedPayload.BuildAssemblies)
        {
            managedPayload.BuildAssemblies();
        }
    }

    void InteropService::LoadAssemblies()
    {
        if (managedPayload.LoadAssemblies)
        {
            managedPayload.LoadAssemblies();
        }
    }

    void InteropService::UnloadAssemblies()
    {
        if (managedPayload.UnloadAssemblies)
        {
            managedPayload.UnloadAssemblies();
        }
    }

    void InteropService::BuildAndLoadAssemblies()
    {
        if (managedPayload.BuildAndLoadAssemblies)
        {
            managedPayload.BuildAndLoadAssemblies();
        }
    }

    void InteropService::Log(LogLevel Level, const char* Message)
    {
        switch (Level)
        {
        case LogLevel::Info:
            Log::Logger()->info(Message);
            break;
        case LogLevel::Warning:
            Log::Logger()->warn(Message);
            break;
        case LogLevel::Error:
            Log::Logger()->error(Message);
            break;
        case LogLevel::Fatal:
            Log::Logger()->critical(Message);
            break;
        }
    }

    void InteropService::Exception(const char* Message)
    {
        Log::Logger()->error(Message);
    }

}
