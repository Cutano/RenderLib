using RenderCore.Interop;

namespace RenderCore.FrameGraph;

public class ResourceManager
{
    private readonly Dictionary<PassNode, PassResource> _resourceRegistry = new();
    private readonly RenderDevice _device = RenderDevice.GetDevice();

    public ResourceManager()
    {
        
    }

    public PassResource GetPassResource(PassNode passNode) => _resourceRegistry[passNode];

    public ResourceManager AddPassResource(PassNode passNode, PassResource resource)
    {
        _resourceRegistry[passNode] = resource;

        return this;
    }

    public void ConstructResource(ResourceNode resourceNode)
    {
        
    }

    public void DisposeResource(ResourceNode resourceNode)
    {
        
    }
}