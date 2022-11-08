using RenderCore.Interop;

namespace RenderCore;

public class SwapChain : ISwapChain
{
    public new SwapChainDesc Desc => GetDesc();
    
    public SwapChain(IntPtr nativePtr) : base(nativePtr)
    {
    }

    public new unsafe SwapChainDesc GetDesc()
    {
        var result = *((delegate* unmanaged[Thiscall]<IntPtr, SwapChainDesc*> )(*(void***) NativePointer)[5])(NativePointer);
        return result;
    }
}