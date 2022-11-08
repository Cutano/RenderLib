using RenderCore.Interop;

namespace RenderCore;

public unsafe class RenderDevice : IRenderDevice
{
    public static delegate* unmanaged<IntPtr> GetDeviceUnmanaged;
    
    private static RenderDevice? _device;
    
    public new RenderDeviceInfo DeviceInfo => GetDeviceInfo();
    public new GraphicsAdapterInfo AdapterInfo => GetAdapterInfo();

    public RenderDevice(IntPtr nativePtr) : base(nativePtr)
    {
    }
    
    public static RenderDevice GetDevice()
    {
        return _device ??= new RenderDevice(GetDeviceUnmanaged());
    }
    
    public new RenderDeviceInfo GetDeviceInfo()
    {
        var result = *((delegate* unmanaged[Thiscall]<IntPtr, RenderDeviceInfo*> )(*(void***) NativePointer)[23])(NativePointer);
        return result;
    }
    
    public new GraphicsAdapterInfo GetAdapterInfo()
    {
        var res = new GraphicsAdapterInfo();
        var result = *((delegate* unmanaged[Thiscall]<IntPtr, GraphicsAdapterInfo.__Native*> )(*(void***) NativePointer)[24])(NativePointer);
        res.__MarshalFrom(ref result);
        return res;
    }
    
    public new TextureFormatInfo GetTextureFormatInfo(TextureFormat texFormat)
    {
        var res = new TextureFormatInfo();
        var result = *((delegate* unmanaged[Thiscall]<IntPtr, short, TextureFormatInfo.__Native*> )(*(void***) NativePointer)[25])(NativePointer, unchecked((short)texFormat));
        res.__MarshalFrom(ref result);
        return res;
    }
    
    public new TextureFormatInfoExt GetTextureFormatInfoExt(TextureFormat texFormat)
    {
        var result = *((delegate* unmanaged[Thiscall]<IntPtr, short, TextureFormatInfoExt*> )(*(void***) NativePointer)[26])(NativePointer, unchecked((short)texFormat));
        return result;
    }
}