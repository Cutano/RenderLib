using System.Diagnostics;
using System.Reflection;
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
    
    private delegate* unmanaged<IntPtr> _workspaceGetAppPath;
    private delegate* unmanaged<IntPtr> _workspaceGetWorkspaceDir;

    private bool _isBuilding;
    private bool _isLoading;

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

    internal void Init(Entry.UnmanagedFunctionPayload payload)
    {
        _workspaceGetAppPath = payload.WorkspaceGetAppPath;
        _workspaceGetWorkspaceDir = payload.WorkspaceGetWorkspaceDir;

        if (!File.Exists(Path.Combine(GetWorkspaceDir(), "ScriptLibrary.csproj")))
        {
            GenerateCsproj();
        }
    }

    internal string GetAppPath()
    {
        return Marshal.PtrToStringAnsi(_workspaceGetAppPath()) ?? string.Empty;
    }

    internal string GetWorkspaceDir()
    {
        return Marshal.PtrToStringAnsi(_workspaceGetWorkspaceDir()) ?? string.Empty;
    }

    internal string GetCsprojPath()
    {
        return Path.Combine(GetWorkspaceDir(), "ScriptLibrary.csproj");
    }

    [UnmanagedCallersOnly]
    internal static void OnCsharpFileChanged(FileAction action, IntPtr path)
    {
        var pathStr = Marshal.PtrToStringUni(path) ?? string.Empty;
        Marshal.FreeBSTR(path);
        Console.WriteLine(pathStr);
    }

    [UnmanagedCallersOnly]
    internal static void RecompileAssembly()
    {
        if (Instance._isBuilding)
        {
            Log.Warning("Still building, wait until done.");
            return;
        }
        
        Task.Run(BuildAssembly);
    }

    [UnmanagedCallersOnly]
    internal static void ReloadAssembly()
    {
        if (Instance._isLoading)
        {
            Log.Warning("Still loading, wait until done.");
            return;
        }
        
        Task.Run(LoadAssembly);
    }

    private static void BuildAssembly()
    {
        Instance._isBuilding = true;
        Log.Information("Building..");
            
        var logger = new MSBuildSerilog();

        using (var process = new Process())
        {
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

    private static void LoadAssembly()
    {
        Instance._isLoading = true;

        var path = Path.Combine(Instance.GetWorkspaceDir(), "Library", "Debug", "ScriptLibrary.dll");
        if (!File.Exists(path))
        {
            Log.Error("Assembly not found, please compile first and check if any errors!");
            Instance._isLoading = false;
            return;
        }

        Log.Information($"Loading {path}...");
        var context = new ScriptLoadContext(path);

        try
        {
            var asm = context.LoadFromAssemblyName(AssemblyName.GetAssemblyName(path));
            
            var availableTypes = string.Join(",", asm.GetTypes().Select(t => t.FullName));
            Log.Information($"Available types: {availableTypes}");

            var scripts = CreateScripts<ScriptBase>(asm).ToList();
            var updatables = CreateScripts<IUpdatable>(asm).ToList();
            var renderables = CreateScripts<IRenderable>(asm).ToList();

            ScriptingCore.Instance.Scripts = scripts;
            ScriptingCore.Instance.Updatables = updatables;
            ScriptingCore.Instance.Renderables = renderables;

            Log.Information($"{scripts.Count} scripts loaded.");
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

    private static void UnloadAssembly()
    {
        
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
        
        var root = ProjectRootElement.Create();
        root.Sdk = "Microsoft.NET.Sdk";
        
        var propertyGroup = root.AddPropertyGroup();
        propertyGroup.AddProperty("TargetFramework", "net6.0");
        propertyGroup.AddProperty("BaseOutputPath", @"Library\");
        propertyGroup.AddProperty("EnableDynamicLoading", "true");
        propertyGroup.AddProperty("AppendTargetFrameworkToOutputPath", "false");

        var itemGroup = root.AddItemGroup();
        var refItem = itemGroup.AddItem("Reference", "ScriptingCore");
        refItem.AddMetadata("HintPath", scriptingInterfaceDllPath);
        refItem.AddMetadata("Private", "false");
        refItem.AddMetadata("ExcludeAssets", "runtime");
        
        root.Save(Path.Combine(GetWorkspaceDir(), "ScriptLibrary.csproj"));
    }
    
    private sealed class MSBuildSerilog : Microsoft.Build.Framework.ILogger
    {
        public void Initialize(IEventSource eventSource)
        {
            eventSource.BuildStarted += (sender, args) =>
            {
                Log.Information(args.Message);
            };
            
            eventSource.BuildFinished += (sender, args) =>
            {
                Log.Information(args.Message);
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