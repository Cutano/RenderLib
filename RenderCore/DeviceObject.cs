using System.Runtime.InteropServices;
using RenderCore.Interop;

namespace RenderCore;

public class DeviceObject : IDeviceObject
{
    public new DeviceObjectAttribs Desc => GetDesc();
    
    public DeviceObject(IntPtr nativePtr) : base(nativePtr)
    {
    }
    
    public new unsafe DeviceObjectAttribs GetDesc()
    {
        var ptr = base.GetDesc();
        var desc = Marshal.PtrToStructure<DeviceObjectAttribs>(ptr);
        return desc;
    }
}