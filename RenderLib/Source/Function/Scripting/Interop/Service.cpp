#include "Base.h"
#include "Service.h"
#include "Bindings.h"
#include "Platform/Workspace/Workspace.h"

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

        rc = loadAssemblyAndGetFunctionPointer(
            dotnetLibPath.c_str(),
            dotnet_type,
            L"ManagedCommand" /*method_name*/,
            UNMANAGEDCALLERSONLY_METHOD,
            nullptr,
            reinterpret_cast<void**>(&ManagedCommand));

        RL_ASSERT(rc == 0 && ManagedCommand != nullptr, "Failure: load_assembly_and_get_function_pointer()")

        if (ManagedCommand)
        {
            int32_t position = 0;
            int32_t checksum = 0;

            // Non-instantiable

            {
                
            }

            // Instantiable

            {
                int32_t head = 0;
                Shared::Functions[position++] = Shared::BottomLevelASFunctions;

                Shared::BottomLevelASFunctions[head++] = (void*)&Binding::BottomLevelAS::GetDesc;
                Shared::BottomLevelASFunctions[head++] = (void*)&Binding::BottomLevelAS::GetGeometryDescIndex;
                Shared::BottomLevelASFunctions[head++] = (void*)&Binding::BottomLevelAS::GetGeometryIndex;
                Shared::BottomLevelASFunctions[head++] = (void*)&Binding::BottomLevelAS::GetActualGeometryCount;
                Shared::BottomLevelASFunctions[head++] = (void*)&Binding::BottomLevelAS::GetScratchBufferSizes;
                Shared::BottomLevelASFunctions[head++] = (void*)&Binding::BottomLevelAS::GetNativeHandle;
                Shared::BottomLevelASFunctions[head++] = (void*)&Binding::BottomLevelAS::SetState;
                Shared::BottomLevelASFunctions[head++] = (void*)&Binding::BottomLevelAS::GetState;

                checksum += head;
            }

            checksum += position;

            // Runtime

            Shared::RuntimeFunctions[0] = (void*)&Exception;
            Shared::RuntimeFunctions[1] = (void*)&Log;

            constexpr void* functions[3] = {
                Shared::RuntimeFunctions,
                Shared::Events,
                Shared::Functions
            };

            if (reinterpret_cast<intptr_t>(ManagedCommand(Command(functions, checksum))) == 0xF) {
                Log::Logger()->info("Host runtime assembly initialized successfully!");
            } else {
                Log::Logger()->error("Host runtime assembly initialized failed!");
                return;
            }

            Status = StatusType::Idle;
            
            Log::Logger()->info("Host loaded successfully!");
        }
        else
        {
            Log::Logger()->error("Host runtime assembly unable to load the initialization function!");
        }
    }

    void InteropService::Shutdown()
    {
        
    }

    size_t Utility::Strcpy(char* Destination, const char* Source, size_t Length) {
        char* destination = Destination;
        const char* source = Source;
        size_t length = Length;

        if (length != 0 && --length != 0) {
            do {
                if ((*destination++ = *source++) == 0)
                    break;
            }

            while (--length != 0);
        }

        if (length == 0) {
            if (Length != 0)
                *destination = '\0';

            while (*source++);
        }

        return (source - Source - 1);
    }

    size_t Utility::Strlen(const char* Source) {
        return strlen(Source) + 1;
    }

}
