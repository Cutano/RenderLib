using RenderCore.Interop;

namespace RenderCore;

public class RenderPass : IRenderPass
{
    public RenderPass(IntPtr nativePtr) : base(nativePtr)
    {
    }
}