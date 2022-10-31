using System.Runtime.InteropServices;
using Microsoft.Build.Construction;

namespace ScriptingCore;

internal unsafe class Workspace
{
    internal static Workspace Instance { get; } = new();
    
    private delegate* unmanaged<IntPtr> _workspaceGetAppPath;
    private delegate* unmanaged<IntPtr> _workspaceGetWorkspaceDir;

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

    [UnmanagedCallersOnly]
    internal static void OnCsharpFileChanged(FileAction action, IntPtr path)
    {
        var pathStr = Marshal.PtrToStringUni(path) ?? string.Empty;
        Marshal.FreeBSTR(path);
        Console.WriteLine(pathStr);
    }

    private void GenerateCsproj()
    {
        var appPath = GetAppPath();
        var scriptingEngineDllPath = Path.Combine(Path.GetDirectoryName(appPath) ?? string.Empty, "net6.0", "ScriptingCore.dll");
        
        var root = ProjectRootElement.Create();
        root.Sdk = "Microsoft.NET.Sdk";
        
        var propertyGroup = root.AddPropertyGroup();
        propertyGroup.AddProperty("TargetFramework", "net6.0");
        propertyGroup.AddProperty("BaseOutputPath", @"Library\");
        propertyGroup.AddProperty("Platforms", "x64");
        propertyGroup.AddProperty("EnableDynamicLoading", "true");
        propertyGroup.AddProperty("AppendTargetFrameworkToOutputPath", "false");

        var itemGroup = root.AddItemGroup();
        var refItem = itemGroup.AddItem("Reference", "ScriptingCore");
        refItem.AddMetadata("HintPath", scriptingEngineDllPath);
        
        root.Save(Path.Combine(GetWorkspaceDir(), "ScriptLibrary.csproj"));
    }
}