using RenderCore.Interop;

namespace RenderCore;

public static unsafe class Device
{
    public static delegate* unmanaged<IntPtr> GetDeviceUnmanaged;
    public static delegate* unmanaged<IntPtr> GetDeviceContextUnmanaged;

    private static IRenderDevice? _device;
    private static IDeviceContext? _deviceContext;

    public static IRenderDevice GetDevice()
    {
        return _device ??= new IRenderDevice(GetDeviceUnmanaged());
    }

    public static IDeviceContext GetContext()
    {
        return _deviceContext ??= new IDeviceContext(GetDeviceContextUnmanaged());
    }
}