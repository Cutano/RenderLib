using RenderCore.Interop;

namespace RenderCore;

public class ShaderResourceBinding : IShaderResourceBinding
{
    public ShaderResourceBinding(IntPtr nativePtr) : base(nativePtr)
    {
    }
}