using RenderCore.Interop;

namespace RenderCore;

public class PipelineStateCache : IPipelineStateCache
{
    public PipelineStateCache(IntPtr nativePtr) : base(nativePtr)
    {
    }
}