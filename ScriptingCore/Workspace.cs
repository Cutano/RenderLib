using System.Runtime.InteropServices;

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
    }

    public string GetAppPath()
    {
        return Marshal.PtrToStringUni(_workspaceGetAppPath()) ?? string.Empty;
    }

    public string GetWorkspaceDir()
    {
        return Marshal.PtrToStringUni(_workspaceGetWorkspaceDir()) ?? string.Empty;
    }
}