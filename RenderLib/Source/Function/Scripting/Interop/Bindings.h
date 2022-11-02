#pragma once
#include "BottomLevelAS.h"

using namespace Diligent;

namespace RL::Interop::Binding
{

    // Instantiable

    namespace BottomLevelAS
    {
        static BottomLevelASDesc* GetDesc(IBottomLevelAS* BottomLevelAS);
        static uint32_t GetGeometryDescIndex(IBottomLevelAS* BottomLevelAS, const char* name);
        static uint32_t GetGeometryIndex(IBottomLevelAS* BottomLevelAS, const char* name);
        static uint32_t GetActualGeometryCount(IBottomLevelAS* BottomLevelAS);
        static ScratchBufferSizes GetScratchBufferSizes(IBottomLevelAS* BottomLevelAS);
        static uint64_t GetNativeHandle(IBottomLevelAS* BottomLevelAS);
        static void SetState(IBottomLevelAS* BottomLevelAS, RESOURCE_STATE State);
        static RESOURCE_STATE GetState(IBottomLevelAS* BottomLevelAS);
    }
}
