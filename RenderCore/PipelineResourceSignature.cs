using RenderCore.Interop;

namespace RenderCore;

public class PipelineResourceSignature : IPipelineResourceSignature
{
    public PipelineResourceSignature(IntPtr nativePtr) : base(nativePtr)
    {
    }
}