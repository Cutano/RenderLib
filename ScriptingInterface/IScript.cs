namespace ScriptingInterface;

public interface IScript : IUpdatable, IRenderable
{
    void Init();

    void Shutdown()
    {
        
    }
}