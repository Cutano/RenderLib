using System.Runtime.InteropServices;
using RenderCore.Interop;

namespace RenderCore;

public class EngineFactory : IEngineFactory
{
    public new APIInfo APIInfo => GetAPIInfo();
    
    public EngineFactory(IntPtr nativePtr) : base(nativePtr)
    {
    }

    public new APIInfo GetAPIInfo()
    {
        var ptr = base.GetAPIInfo();
        var info = Marshal.PtrToStructure<APIInfo>(ptr);
        return info;
    }
}