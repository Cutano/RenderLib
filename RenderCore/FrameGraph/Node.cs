namespace RenderCore.FrameGraph;

public abstract class Node
{
    public string Name { get; init; } = "Unnamed Node";
}

public class PassNode : Node
{
    public enum Type { General, Copy }
    
    public bool Async { get; init; }
    public Type PassType { get; init; }
    public List<ResourceNode> Inputs { get; init; }
    public List<ResourceNode> Outputs { get; init; }

    public PassNode(string name, List<ResourceNode> inputs, List<ResourceNode> outputs, bool async = false, Type passType = Type.General)
    {
        Name = name;
        PassType = passType;
        Async = async;
        Inputs = inputs;
        Outputs = outputs;
    }

    public bool IsValid()
    {
        if (PassType == Type.Copy && Inputs.Count != Outputs.Count)
        {
            return false;
        }

        return true;
    }
}

public class ResourceNode : Node
{
    public ResourceNode(string name)
    {
        Name = name;
    }
}

public class MoveNode : Node
{
    public ResourceNode Source { get; init; }
    public ResourceNode Destination { get; init; }

    public MoveNode(string name, ResourceNode source, ResourceNode destination)
    {
        Name = name;
        Source = source;
        Destination = destination;
    }
}