using RenderCore.Interop;

namespace RenderCore;

public class CommandQueue : ICommandQueue
{
    public CommandQueue(IntPtr nativePtr) : base(nativePtr)
    {
    }
}