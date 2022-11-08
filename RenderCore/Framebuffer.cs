using RenderCore.Interop;

namespace RenderCore;

public class Framebuffer : IFramebuffer
{
    public Framebuffer(IntPtr nativePtr) : base(nativePtr)
    {
    }
}