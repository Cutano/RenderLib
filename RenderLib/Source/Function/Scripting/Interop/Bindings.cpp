#include "Base.h"
#include "Bindings.h"
#include "InteropService.h"
#include "Platform/Workspace/Workspace.h"

namespace RL::Interop::Binding
{
    namespace Workspace
    {
        char* GetAppPath()
        {
            return RL::Workspace::Get().GetApplicationPath().data();
        }

        char* GetWorkspaceDir()
        {
            return RL::Workspace::Get().GetWorkspaceDir().data();
        }
    }
}
