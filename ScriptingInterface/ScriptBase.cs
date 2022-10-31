namespace ScriptingInterface;

public abstract class ScriptBase : IScript
{
    public abstract void Init();
    
    public abstract void Update(double dt);

    public abstract void Render(RenderContext context);

    public bool Active { get; set; } = true;
}