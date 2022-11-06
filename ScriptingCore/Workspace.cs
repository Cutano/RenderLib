using System.Diagnostics;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Microsoft.Build.Construction;
using Microsoft.Build.Evaluation;
using Microsoft.Build.Execution;
using Microsoft.Build.Framework;
using ScriptingInterface;
using Serilog;

namespace ScriptingCore;

internal unsafe class Workspace
{
    internal static Workspace Instance { get; } = new();
    
    internal static delegate* unmanaged<IntPtr> WorkspaceGetAppPath;
    internal static delegate* unmanaged<IntPtr> WorkspaceGetWorkspaceDir;

    private bool _isBuilding;
    private bool _isLoading;

    private ScriptLoadContext? _scriptLoadContext = null;

    internal enum FileAction
    {
        /// Sent when a file is created or renamed
        Add = 1,
        /// Sent when a file is deleted or renamed
        Delete = 2,
        /// Sent when a file is modified
        Modified = 3,
        /// Sent when a file is moved
        Moved = 4
    }
        
    static Workspace()
    {
    }

    private Workspace()
    {
            
    }

    internal void Init()
    {
        if (!File.Exists(Path.Combine(GetWorkspaceDir(), "ScriptLibrary.csproj")))
        {
            GenerateCsproj();
        }
    }

    internal string GetAppPath()
    {
        return Marshal.PtrToStringAnsi(WorkspaceGetAppPath()) ?? string.Empty;
    }

    internal string GetWorkspaceDir()
    {
        return Marshal.PtrToStringAnsi(WorkspaceGetWorkspaceDir()) ?? string.Empty;
    }

    internal string GetCsprojPath()
    {
        return Path.Combine(GetWorkspaceDir(), "ScriptLibrary.csproj");
    }

    [UnmanagedCallersOnly]
    internal static void OnSourceFileChanged(FileAction action, IntPtr path)
    {
        var pathStr = Marshal.PtrToStringUni(path) ?? string.Empty;
        Console.WriteLine(pathStr);
    }

    [UnmanagedCallersOnly]
    internal static void BuildAssemblies()
    {
        if (Instance._isBuilding)
        {
            Log.Warning("Still building, wait until done.");
            return;
        }
        
        Task.Run(BuildAssembly);
    }
    
    [UnmanagedCallersOnly]
    internal static void LoadAssemblies()
    {
        if (Instance._isLoading)
        {
            Log.Warning("Still loading, wait until done.");
            return;
        }
        
        Task.Run(LoadAssembly);
    }
    
    [UnmanagedCallersOnly]
    internal static void UnloadAssemblies()
    {
        UnloadAssembly();
    }
    
    [UnmanagedCallersOnly]
    internal static void BuildAndLoadAssemblies()
    {
        
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    private static void BuildAssembly()
    {
        Log.Info("Building..");
        Instance._isBuilding = true;
        UnloadAssembly();

        var logger = new MSBuildSerilog();

        if (!File.Exists(Path.Combine(Instance.GetWorkspaceDir(), "obj", "project.assets.json")))
        {
            using var process = new Process();
            process.StartInfo = new ProcessStartInfo
            {
                FileName = "dotnet.exe",
                Arguments = $"restore {Instance.GetWorkspaceDir()}",
                UseShellExecute = false,
                CreateNoWindow = true
            };

            process.Start();
            process.WaitForExit();
        }

        var projectCollection = new ProjectCollection();
        var buildParams = new BuildParameters(projectCollection)
        {
            Loggers = new List<Microsoft.Build.Framework.ILogger> { logger }
        };

        var globalProperty = new Dictionary<string, string>
        {
            { "Configuration", "Debug" }
        };

        BuildManager.DefaultBuildManager.ResetCaches();
        var path = Instance.GetCsprojPath();
        var buildRequest = new BuildRequestData(path, globalProperty, null, new[] { "Build" }, null);
        var buildResult = BuildManager.DefaultBuildManager.Build(buildParams, buildRequest);
        if (buildResult.OverallResult == BuildResultCode.Failure)
        {
            Log.Error("Failed compiling scripts!");
        }
            
        Instance._isBuilding = false;
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    private static void LoadAssembly()
    {
        Instance._isLoading = true;
        UnloadAssembly();

        var path = Path.Combine(Instance.GetWorkspaceDir(), "Library", "Debug", "ScriptLibrary.dll");
        if (!File.Exists(path))
        {
            Log.Error("Assembly not found, please compile first and check if any errors!");
            Instance._isLoading = false;
            return;
        }

        Log.Info($"Loading {path}...");
        Instance._scriptLoadContext = new ScriptLoadContext(path);

        try
        {
            var asm = Instance._scriptLoadContext.LoadFromAssemblyName(AssemblyName.GetAssemblyName(path));
            
            var availableTypes = string.Join(",", asm.GetTypes().Select(t => t.FullName));
            Log.Info($"Available types: {availableTypes}");

            var scripts = CreateScripts<ScriptBase>(asm).ToList();
            var updatables = CreateScripts<IUpdatable>(asm).ToList();
            var renderables = CreateScripts<IRenderable>(asm).ToList();

            ScriptingCore.Instance.Scripts = scripts;
            ScriptingCore.Instance.Updatables = updatables;
            ScriptingCore.Instance.Renderables = renderables;

            Log.Info($"{scripts.Count} scripts loaded.");
        }
        catch (Exception e)
        {
            Log.Error(e.Message);
            throw;
        }
        finally
        {
            Instance._isLoading = false;
        }
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    private static void UnloadAssembly()
    {
        ScriptingCore.Instance.Scripts = new List<ScriptBase>();
        ScriptingCore.Instance.Updatables = new List<IUpdatable>();
        ScriptingCore.Instance.Renderables = new List<IRenderable>();

        var alcWeakRef = new WeakReference(Instance._scriptLoadContext, trackResurrection: true);
        
        Instance._scriptLoadContext?.Unload();
        Instance._scriptLoadContext = null;
        
        for (var i = 0; alcWeakRef.IsAlive && i < 10; i++)
        {
            GC.Collect();
            GC.WaitForPendingFinalizers();
        }
    }

    private static IEnumerable<T> CreateScripts<T>(Assembly asm)
    {
        foreach (var type in asm.GetTypes())
        {
            if (typeof(T).IsAssignableFrom(type))
            {
                if (Activator.CreateInstance(type) is T script)
                {
                    yield return script;
                }
            }
        }
    }

    private void GenerateCsproj()
    {
        var appPath = GetAppPath();
        var scriptingInterfaceDllPath = Path.Combine(Path.GetDirectoryName(appPath) ?? string.Empty, "ScriptingInterface.dll");
        var renderCoreDllPath = Path.Combine(Path.GetDirectoryName(appPath) ?? string.Empty, "RenderCore.dll");
        var sharpGenRuntimeDllPath = Path.Combine(Path.GetDirectoryName(appPath) ?? string.Empty, "SharpGen.Runtime.dll");
        
        var root = ProjectRootElement.Create();
        root.Sdk = "Microsoft.NET.Sdk";
        
        var propertyGroup = root.AddPropertyGroup();
        propertyGroup.AddProperty("TargetFramework", "net6.0");
        propertyGroup.AddProperty("BaseOutputPath", @"Library\");
        propertyGroup.AddProperty("EnableDynamicLoading", "true");
        propertyGroup.AddProperty("AppendTargetFrameworkToOutputPath", "false");
        propertyGroup.AddProperty("Nullable", "enable");

        var itemGroup = root.AddItemGroup();
        
        var scriptingInterface = itemGroup.AddItem("Reference", "ScriptingInterface");
        scriptingInterface.AddMetadata("HintPath", scriptingInterfaceDllPath);
        scriptingInterface.AddMetadata("Private", "false");
        scriptingInterface.AddMetadata("ExcludeAssets", "runtime");
        
        var renderCore = itemGroup.AddItem("Reference", "RenderCore");
        renderCore.AddMetadata("HintPath", renderCoreDllPath);
        renderCore.AddMetadata("Private", "false");
        renderCore.AddMetadata("ExcludeAssets", "runtime");
        
        var sharpGenRuntime = itemGroup.AddItem("Reference", "SharpGen.Runtime");
        sharpGenRuntime.AddMetadata("HintPath", sharpGenRuntimeDllPath);
        sharpGenRuntime.AddMetadata("Private", "false");
        sharpGenRuntime.AddMetadata("ExcludeAssets", "runtime");

        root.Save(Path.Combine(GetWorkspaceDir(), "ScriptLibrary.csproj"));
    }
    
    private sealed class MSBuildSerilog : Microsoft.Build.Framework.ILogger
    {
        public void Initialize(IEventSource eventSource)
        {
            eventSource.BuildStarted += (sender, args) =>
            {
                Log.Info(args.Message);
            };
            
            eventSource.BuildFinished += (sender, args) =>
            {
                Log.Info(args.Message);
            };

            eventSource.ErrorRaised += (sender, args) =>
            {
                Log.Error($"Msg: {args.Message}, Code: {args.Code}, File: {args.File}");
            };
        }

        public void Shutdown()
        {
            
        }

        public LoggerVerbosity Verbosity { get; set; }
        public string? Parameters { get; set; }
    }
}