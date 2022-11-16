namespace RenderCore.FrameGraph;

public delegate void PassFunction(DeviceContext cmd, PassResource resource);

public class Executor
{
    private RenderDevice _device;
    private Dictionary<PassNode, PassFunction> _passFunctionRegistry = new();

    public Executor()
    {
        _device = RenderDevice.GetDevice();
    }

    public Executor RegisterPass(PassNode passNode, PassFunction passFunction)
    {
        _passFunctionRegistry[passNode] = passFunction;
        
        return this;
    }

    public void Execute(Compiler.Result compileResult, ResourceManager manager)
    {
        if (compileResult.SortedPasses is null)
        {
            return;
        }

        for (var index = 0; index < compileResult.SortedPasses.Count; index++)
        {
            var passNode = compileResult.SortedPasses[index];
            
            foreach (var resourceNode in compileResult.PassInfoRegistry[passNode].ConstructedResource)
            {
                manager.ConstructResource(resourceNode);
            }
            
            _passFunctionRegistry[passNode].Invoke(DeviceContext.GetDeferredContext(index), manager.GetPassResource(passNode));

            foreach (var resourceNode in compileResult.PassInfoRegistry[passNode].DisposedResource)
            {
                manager.DisposeResource(resourceNode);
            }
        }
    }
}