using System.Runtime.InteropServices;
using RenderCore.Interop;

namespace RenderCore;

public unsafe class DeviceContext : IDeviceContext
{
    public static delegate* unmanaged<IntPtr> GetDeviceContextUnmanaged;
    public static delegate* unmanaged<IntPtr> GetDeviceDeferredContext;
    public static delegate* unmanaged<int> GetDeviceNumDeferredContext;
    
    private static DeviceContext? _deviceContext;
    private static DeviceContext[]? _deferredContext;
    
    public new DeviceContextDesc Desc => GetDesc();

    public DeviceContext(IntPtr nativePtr) : base(nativePtr)
    {
    }
    
    public static DeviceContext GetContext()
    {
        return _deviceContext ??= new DeviceContext(GetDeviceContextUnmanaged());
    }

    public static DeviceContext GetDeferredContext(int index)
    {
        if (_deferredContext is null)
        {
            var num = GetDeviceNumDeferredContext();
            _deferredContext = new DeviceContext[num];

            var baseAddr = GetDeviceDeferredContext();
            for (var i = 0; i < num; i++)
            {
                _deferredContext[i] = new DeviceContext(baseAddr + i);
            }
        }

        return _deferredContext[index];
    }

    public static int GetNumDeferredContext()
    {
        if (_deferredContext is null)
        {
            return GetDeviceNumDeferredContext();
        }
        
        return _deferredContext.Length;
    }

    public new DeviceContextDesc GetDesc()
    {
        var res = new DeviceContextDesc();
        var result = *((delegate* unmanaged[Thiscall]<IntPtr, DeviceContextDesc.__Native*> )(*(void***) NativePointer)[4])(NativePointer);
        res.__MarshalFrom(ref result);
        return res;
    }
}