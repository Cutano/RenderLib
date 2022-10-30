#include "Base.h"
#include "Bindings.h"
#include "Platform/Workspace/Workspace.h"

namespace RL
{
    char* WorkspaceGetAppPath()
    {
        return Workspace::Get().GetApplicationPath().data();
    }

    char* WorkspaceGetWorkspaceDir()
    {
        return Workspace::Get().GetWorkspaceDir().data();
    }
}
