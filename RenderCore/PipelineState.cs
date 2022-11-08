using System.Runtime.InteropServices;
using RenderCore.Interop;

namespace RenderCore;

public class PipelineState : IPipelineState
{
    public new GraphicsPipelineDesc GraphicsPipelineDesc => GetGraphicsPipelineDesc();
    public new RayTracingPipelineDesc RayTracingPipelineDesc => GetRayTracingPipelineDesc();
    public new TilePipelineDesc TilePipelineDesc => GetTilePipelineDesc();
    
    public PipelineState(IntPtr nativePtr) : base(nativePtr)
    {
    }

    public new unsafe GraphicsPipelineDesc GetGraphicsPipelineDesc()
    {
        var res = new GraphicsPipelineDesc();
        var result = *((delegate* unmanaged[Thiscall]<IntPtr, GraphicsPipelineDesc.__Native*> )(*(void***) NativePointer)[8])(NativePointer);
        res.__MarshalFrom(ref result);
        return res;
        
        // var ptr = base.GetGraphicsPipelineDesc();
        // var desc = Marshal.PtrToStructure<GraphicsPipelineDesc>(ptr);
        // return desc;
    }
    
    public new unsafe RayTracingPipelineDesc GetRayTracingPipelineDesc()
    {
        var result = *((delegate* unmanaged[Thiscall]<IntPtr, RayTracingPipelineDesc*> )(*(void***) NativePointer)[9])(NativePointer);
        return result;
        
        // var ptr = base.GetRayTracingPipelineDesc();
        // var desc = Marshal.PtrToStructure<RayTracingPipelineDesc>(ptr);
        // return desc;
    }
    
    public new unsafe TilePipelineDesc GetTilePipelineDesc()
    {
        var res = new TilePipelineDesc();
        var result = *((delegate* unmanaged[Thiscall]<IntPtr, TilePipelineDesc.__Native*> )(*(void***) NativePointer)[10])(NativePointer);
        res.__MarshalFrom(ref result);
        return res;
        
        // var ptr = base.GetTilePipelineDesc();
        // var desc = Marshal.PtrToStructure<TilePipelineDesc>(ptr);
        // return desc;
    }
}