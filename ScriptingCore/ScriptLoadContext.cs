using System.Reflection;
using System.Runtime.Loader;
using ScriptingInterface;

namespace ScriptingCore;

public class ScriptLoadContext : AssemblyLoadContext
{
    private readonly AssemblyDependencyResolver _resolver;

    public ScriptLoadContext(string path) : base(isCollectible: true)
    {
        _resolver = new AssemblyDependencyResolver(path);
    }

    protected override Assembly? Load(AssemblyName assemblyName)
    {
        // Crucial!
        // This prevents the script assembly from reloading interface DLL into it's own ALC,
        // ensures the interface DLL has only one instance.
        if (assemblyName.FullName == typeof(IScript).Assembly.FullName)
        {
            var loadedAssemblies = AppDomain.CurrentDomain.GetAssemblies();
            var interfaceAsm = loadedAssemblies.First(assembly => assembly.FullName == assemblyName.FullName);
            return interfaceAsm;
        }
        
        var assemblyPath = _resolver.ResolveAssemblyToPath(assemblyName);
        return assemblyPath != null ? LoadFromAssemblyPath(assemblyPath) : null;
    }

    protected override IntPtr LoadUnmanagedDll(string unmanagedDllName)
    {
        var libraryPath = _resolver.ResolveUnmanagedDllToPath(unmanagedDllName);
        return libraryPath != null ? LoadUnmanagedDllFromPath(libraryPath) : IntPtr.Zero;
    }
}