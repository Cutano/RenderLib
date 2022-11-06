#pragma once
#include "BottomLevelAS.h"

using namespace Diligent;

namespace RL::Interop::Binding
{
    // Non-instantiable
    
    namespace Workspace
    {
        typedef void (*SourceFileChangedDelegate)(int action, const char* path);
        
        char* GetAppPath();
        char* GetWorkspaceDir();
        void  SetOnSourceFileChangedCallback(SourceFileChangedDelegate sourceFileChangedDelegate);
    }

    // Instantiable

    
}
