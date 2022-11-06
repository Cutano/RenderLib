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
    }

    // Instantiable

    
}
