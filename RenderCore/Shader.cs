using RenderCore.Interop;

namespace RenderCore;

public class Shader : IShader
{
    public Shader(IntPtr nativePtr) : base(nativePtr)
    {
    }
}