using System.Runtime.InteropServices;
using Microsoft.Build.Locator;

namespace ScriptingCore;

internal static partial class Entry
{
    [UnmanagedCallersOnly]
    // ReSharper disable once UnusedMember.Global
    internal static unsafe ManagedFunctionPayload Init(UnmanagedFunctionPayload unmanagedPayload)
    {
        MSBuildLocator.RegisterDefaults();

        Workspace.WorkspaceGetAppPath = unmanagedPayload.WorkspaceGetAppPath;
        Workspace.WorkspaceGetWorkspaceDir = unmanagedPayload.WorkspaceGetWorkspaceDir;
        RenderCore.RenderDevice.GetDeviceUnmanaged = unmanagedPayload.DeviceGet;
        RenderCore.DeviceContext.GetDeviceContextUnmanaged = unmanagedPayload.DeviceGetContext;
        RenderCore.DeviceContext.GetDeviceDeferredContext = unmanagedPayload.DeviceGetDeferredContext;
        RenderCore.DeviceContext.GetDeviceNumDeferredContext = unmanagedPayload.DeviceGetNumDeferredContext;
        RenderCore.Log.LogUnmanaged = unmanagedPayload.GraphicsLog;
        Log.LogUnmanaged = unmanagedPayload.ScriptCoreLog;

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