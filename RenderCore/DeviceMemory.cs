using RenderCore.Interop;

namespace RenderCore;

public class DeviceMemory : IDeviceMemory
{
    public DeviceMemory(IntPtr nativePtr) : base(nativePtr)
    {
    }
}