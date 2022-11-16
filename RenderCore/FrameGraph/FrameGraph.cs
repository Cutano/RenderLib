using csdot;
using csdot.Attributes.DataTypes;

namespace RenderCore.FrameGraph;

public class FrameGraph
{
    private Compiler _compiler = new();
    private Executor _executor = new();

    public string Name { get; init; }

    public List<MoveNode> MoveNodes { get; private set; } = new();
    public List<PassNode> PassNodes { get; private set; } = new();
    public List<ResourceNode> ResourceNodes { get; private set; } = new();

    public Dictionary<string, PassNode> PassNodeRegistry { get; private set; } = new();
    public Dictionary<string, ResourceNode> ResourceNodeRegistry { get; private set; } = new();
    public Dictionary<string, MoveNode> MoveNodeByName { get; private set; } = new();
    public Dictionary<ResourceNode, MoveNode> MoveNodeBySource { get; private set; } = new();
    public Dictionary<ResourceNode, MoveNode> MoveNodeByDestination { get; private set; } = new();

    public Compiler.Result? CompileResult { get; set; }

    public FrameGraph(string name)
    {
        Name = name;
    }

    public PassNode GetPassNode(string name) => PassNodeRegistry[name];
    public ResourceNode GetResourceNode(string name) => ResourceNodeRegistry[name];
    public MoveNode GetMoveNode(string name) => MoveNodeByName[name];
    public ResourceNode GetMoveDestinationBySource(ResourceNode source) => MoveNodeBySource[source].Destination;
    public ResourceNode GetMoveSourceByDestination(ResourceNode destination) => MoveNodeByDestination[destination].Source;

    public FrameGraph AddResourceNode(ResourceNode resourceNode)
    {
        ResourceNodes.Add(resourceNode);
        ResourceNodeRegistry[resourceNode.Name] = resourceNode;

        return this;
    }

    public FrameGraph AddResourceNode(string name)
    {
        var node = new ResourceNode(name);
        ResourceNodes.Add(node);
        ResourceNodeRegistry[name] = node;

        return this;
    }

    public FrameGraph AddPassNode(PassNode passNode)
    {
        PassNodes.Add(passNode);
        PassNodeRegistry[passNode.Name] = passNode;

        return this;
    }

    public FrameGraph AddPassNode(string name, IEnumerable<ResourceNode> inputs, IEnumerable<ResourceNode> outputs,
        bool async = false, PassNode.Type passType = PassNode.Type.General)
    {
        var node = new PassNode(name, inputs, outputs, async, passType);
        PassNodes.Add(node);
        PassNodeRegistry[name] = node;

        return this;
    }
    
    public FrameGraph AddPassNode(string name, IEnumerable<string> inputsName, IEnumerable<string> outputsName,
        bool async = false, PassNode.Type passType = PassNode.Type.General)
    {
        var inputs = from inputName in inputsName select ResourceNodeRegistry[inputName];
        var outputs = from outputName in outputsName select ResourceNodeRegistry[outputName];
        var node = new PassNode(name, inputs, outputs, async, passType);
        PassNodes.Add(node);
        PassNodeRegistry[name] = node;

        return this;
    }

    public FrameGraph AddMoveNode(MoveNode moveNode)
    {
        MoveNodes.Add(moveNode);
        MoveNodeByName[moveNode.Name] = moveNode;
        MoveNodeBySource[moveNode.Source] = moveNode;
        MoveNodeByDestination[moveNode.Destination] = moveNode;

        return this;
    }

    public FrameGraph AddMoveNode(string name, ResourceNode source, ResourceNode destination)
    {
        var moveNode = new MoveNode(name, source, destination);
        MoveNodes.Add(moveNode);
        MoveNodeByName[name] = moveNode;
        MoveNodeBySource[source] = moveNode;
        MoveNodeByDestination[destination] = moveNode;

        return this;
    }

    public void Compile()
    {
        CompileResult = _compiler.Compile(this);
    }

    public void Execute(ResourceManager manager)
    {
        if (CompileResult is not null)
        {
            _executor.Execute(CompileResult, manager);
        }
        else
        {
            Log.Error("No available compile result!");
        }
    }

    public void ToGraphVis()
    {
        var doc = new DotDocument();
        
        var graph = new Graph(Name)
        {
            strict = true,
            type = "graph"
        };

        foreach (var passNode in PassNodes)
        {
            foreach (var input in passNode.Inputs)
            {
                var inputTransition = new List<Transition>
                {
                    new(passNode.Name, EdgeOp.directed)
                };

                graph.AddElements(new csdot.Node(input.Name), new Edge(inputTransition));
            }

            var outputTransition = passNode.Outputs.Select(
                output => new Transition(output.Name, EdgeOp.directed)).ToList();
            graph.AddElements(new csdot.Node(passNode.Name), new Edge(outputTransition));
        }

        doc.SaveToFile(graph, "FrameGraph.dot");
    }
}