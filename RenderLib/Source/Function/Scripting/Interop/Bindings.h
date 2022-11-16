#pragma once

namespace RL::Interop::Binding
{
    // Non-instantiable
    
    namespace Workspace
    {
        char* GetAppPath();
        char* GetWorkspaceDir();
    }

    namespace Device
    {
        void* GetDevice();
        void* GetContext();
        void* GetDeferredContext();
        uint32_t GetNumDeferredContext();
    }

    // Instantiable

    
}
