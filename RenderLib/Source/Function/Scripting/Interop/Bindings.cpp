#include "Bindings.h"

namespace RL::Interop::Binding
{
    namespace BottomLevelAS
    {
        BottomLevelASDesc* GetDesc(IBottomLevelAS* BottomLevelAS)
        {
            return &const_cast<BottomLevelASDesc&>(BottomLevelAS->GetDesc());
        }

        uint32_t GetGeometryDescIndex(IBottomLevelAS* BottomLevelAS, const char* name)
        {
            return BottomLevelAS->GetGeometryIndex(name);
        }

        uint32_t GetGeometryIndex(IBottomLevelAS* BottomLevelAS, const char* name)
        {
            return BottomLevelAS->GetGeometryIndex(name);
        }

        uint32_t GetActualGeometryCount(IBottomLevelAS* BottomLevelAS)
        {
            return BottomLevelAS->GetActualGeometryCount();
        }

        ScratchBufferSizes GetScratchBufferSizes(IBottomLevelAS* BottomLevelAS)
        {
            return BottomLevelAS->GetScratchBufferSizes();
        }

        uint64_t GetNativeHandle(IBottomLevelAS* BottomLevelAS)
        {
            return BottomLevelAS->GetNativeHandle();
        }

        void SetState(IBottomLevelAS* BottomLevelAS, RESOURCE_STATE State)
        {
            BottomLevelAS->SetState(State);
        }

        RESOURCE_STATE GetState(IBottomLevelAS* BottomLevelAS)
        {
            return BottomLevelAS->GetState();
        }
    }
}
