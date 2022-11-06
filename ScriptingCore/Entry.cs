using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Build.Locator;
using Serilog;

namespace ScriptingCore;

internal static partial class Entry
{
    [UnmanagedCallersOnly]
    // ReSharper disable once UnusedMember.Global
    internal static unsafe ManagedFunctionPayload Init(UnmanagedFunctionPayload unmanagedPayload)
    {
        Log.Logger = new LoggerConfiguration()
            .MinimumLevel.Information()
            .WriteTo.Console()
            .CreateLogger();

        MSBuildLocator.RegisterDefaults();

        Workspace.WorkspaceGetAppPath = unmanagedPayload.WorkspaceGetAppPath;
        Workspace.WorkspaceGetWorkspaceDir = unmanagedPayload.WorkspaceGetWorkspaceDir;
            
        Workspace.Instance.Init();
        ScriptingCore.Instance.Init();

        ManagedFunctionPayload managedPayload;
        managedPayload.UpdateManaged = &Update;
        managedPayload.RenderManaged = &Render;
        managedPayload.OnSourceFileChangedManaged = &Workspace.OnSourceFileChanged;
        managedPayload.BuildAssembliesManaged = &Workspace.BuildAssemblies;
        managedPayload.LoadAssembliesManaged = &Workspace.LoadAssemblies;
        managedPayload.UnloadAssembliesManaged = &Workspace.UnloadAssemblies;
        managedPayload.BuildAndLoadAssembliesManaged = &Workspace.BuildAndLoadAssemblies;
        return managedPayload;
    }

    [UnmanagedCallersOnly]
    internal static void Update(double dt)
    {
        ScriptingCore.Instance.Update(dt);
    }
    
    [UnmanagedCallersOnly]
    internal static void Render()
    {
        // ScriptingCore.Instance.Render();
    }
}