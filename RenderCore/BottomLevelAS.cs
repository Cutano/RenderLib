using RenderCore.Interop;

namespace RenderCore;

public class BottomLevelAS : IBottomLevelAS
{
    public BottomLevelAS(IntPtr nativePtr) : base(nativePtr)
    {
    }
}