using RenderCore.Interop;

namespace RenderCore;

public class Buffer : IBuffer
{
    public Buffer(IntPtr nativePtr) : base(nativePtr)
    {
    }
}