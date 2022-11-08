using RenderCore.Interop;

namespace RenderCore;

public class CommandList : ICommandList
{
    public CommandList(IntPtr nativePtr) : base(nativePtr)
    {
    }
}