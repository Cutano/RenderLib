namespace ScriptingInterface;

public interface IDummy
{
    string Name { get; }
    string Description { get; }

    int Execute();
}