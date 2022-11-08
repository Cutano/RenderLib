using RenderCore.Interop;

namespace RenderCore;

public class BufferView : IBufferView
{
    public BufferView(IntPtr nativePtr) : base(nativePtr)
    {
    }
}