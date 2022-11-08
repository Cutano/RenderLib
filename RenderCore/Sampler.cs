using RenderCore.Interop;

namespace RenderCore;

public class Sampler : ISampler
{
    public Sampler(IntPtr nativePtr) : base(nativePtr)
    {
    }
}