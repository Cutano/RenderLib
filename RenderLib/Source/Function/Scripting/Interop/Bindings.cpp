#include "Base.h"
#include "Bindings.h"
#include "InteropService.h"
#include "Function/Graphics/GraphicEngine.h"
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

    namespace Device
    {
        void* GetDevice()
        {
            return GraphicEngine::Get().GetDevice();
        }

        void* GetContext()
        {
            return GraphicEngine::Get().GetDeviceContext();
        }
    }
}
