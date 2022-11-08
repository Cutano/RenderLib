using RenderCore.Interop;

namespace RenderCore;

public class Fence : IFence
{
    public Fence(IntPtr nativePtr) : base(nativePtr)
    {
    }
}