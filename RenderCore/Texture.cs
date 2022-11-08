using RenderCore.Interop;

namespace RenderCore;

public class Texture : ITexture
{
    public new SparseTextureProperties SparseProperties => GetSparseProperties();
    
    public Texture(IntPtr nativePtr) : base(nativePtr)
    {
    }

    public new unsafe SparseTextureProperties GetSparseProperties()
    {
        var res = new SparseTextureProperties();
        var result = *((delegate* unmanaged[Thiscall]<IntPtr, SparseTextureProperties.__Native*> )(*(void***) NativePointer)[13])(NativePointer);
        res.__MarshalFrom(ref result);
        return res;
    }
}