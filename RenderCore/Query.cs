using RenderCore.Interop;

namespace RenderCore;

public class Query : IQuery
{
    public Query(IntPtr nativePtr) : base(nativePtr)
    {
    }
}