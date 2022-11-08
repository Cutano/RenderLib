using RenderCore.Interop;

namespace RenderCore;

public class ShaderBindingTable : IShaderBindingTable
{
    public ShaderBindingTable(IntPtr nativePtr) : base(nativePtr)
    {
    }
}