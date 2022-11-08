using RenderCore.Interop;

namespace RenderCore;

public class ShaderResourceVariable : IShaderResourceVariable
{
    public ShaderResourceVariable(IntPtr nativePtr) : base(nativePtr)
    {
    }
}