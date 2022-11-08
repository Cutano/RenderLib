using RenderCore.Interop;

namespace RenderCore;

public class TextureView : ITextureView
{
    public TextureView(IntPtr nativePtr) : base(nativePtr)
    {
    }
}