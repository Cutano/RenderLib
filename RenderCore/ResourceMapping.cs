using RenderCore.Interop;

namespace RenderCore;

public class ResourceMapping : IResourceMapping
{
    public ResourceMapping(IntPtr nativePtr) : base(nativePtr)
    {
    }
}