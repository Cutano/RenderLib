using System.Runtime.InteropServices;
using Microsoft.Build.Construction;

namespace ScriptingCore;

public unsafe class Workspace
{
    public static Workspace Instance { get; } = new();
    
    private delegate* unmanaged<IntPtr> _workspaceGetAppPath;
    private delegate* unmanaged<IntPtr> _workspaceGetWorkspaceDir;
        
    static Workspace()
    {
    }

    private Workspace()
    {
            
    }

    public void Init(Entry.UnmanagedFunctionPayload payload)
    {
        _workspaceGetAppPath = payload.WorkspaceGetAppPath;
        _workspaceGetWorkspaceDir = payload.WorkspaceGetWorkspaceDir;

        if (!File.Exists(Path.Combine(GetWorkspaceDir(), "ScriptLibrary.csproj")))
        {
            GenerateCsproj();
        }
    }

    public string GetAppPath()
    {
        return Marshal.PtrToStringAnsi(_workspaceGetAppPath()) ?? string.Empty;
    }

    public string GetWorkspaceDir()
    {
        return Marshal.PtrToStringAnsi(_workspaceGetWorkspaceDir()) ?? string.Empty;
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

        var itemGroup = root.AddItemGroup();
        var refItem = itemGroup.AddItem("Reference", "ScriptingCore");
        refItem.AddMetadata("HintPath", scriptingEngineDllPath);
        
        root.Save(Path.Combine(GetWorkspaceDir(), "ScriptLibrary.csproj"));
    }
}