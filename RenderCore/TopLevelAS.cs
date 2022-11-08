using RenderCore.Interop;

namespace RenderCore;

public class TopLevelAS : ITopLevelAS
{
    public TopLevelAS(IntPtr nativePtr) : base(nativePtr)
    {
    }
}