using System.Runtime.InteropServices;
using RenderCore.Interop;

namespace RenderCore;

public unsafe class DeviceContext : IDeviceContext
{
    public static delegate* unmanaged<IntPtr> GetDeviceContextUnmanaged;
    
    private static DeviceContext? _deviceContext;
    
    public new DeviceContextDesc Desc => GetDesc();

    public DeviceContext(IntPtr nativePtr) : base(nativePtr)
    {
    }
    
    public static DeviceContext GetContext()
    {
        return _deviceContext ??= new DeviceContext(GetDeviceContextUnmanaged());
    }

    public new DeviceContextDesc GetDesc()
    {
        var res = new DeviceContextDesc();
        var result = *((delegate* unmanaged[Thiscall]<IntPtr, DeviceContextDesc.__Native*> )(*(void***) NativePointer)[4])(NativePointer);
        res.__MarshalFrom(ref result);
        return res;
    }
}