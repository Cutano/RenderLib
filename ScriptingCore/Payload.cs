using System.Runtime.InteropServices;

namespace ScriptingCore;

partial class Entry
{
    [StructLayout(LayoutKind.Sequential)]
    internal unsafe struct ManagedFunctionPayload
    {
        internal delegate* unmanaged<double, void> UpdateManaged;
        internal delegate* unmanaged<void> RenderManaged;
        internal delegate* unmanaged<Workspace.FileAction, IntPtr, void> OnSourceFileChangedManaged;
        internal delegate* unmanaged<void> BuildAssembliesManaged;
        internal delegate* unmanaged<void> LoadAssembliesManaged;
        internal delegate* unmanaged<void> UnloadAssembliesManaged;
        internal delegate* unmanaged<void> BuildAndLoadAssembliesManaged;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal unsafe struct UnmanagedFunctionPayload
    {
        internal delegate* unmanaged<IntPtr> WorkspaceGetAppPath;
        internal delegate* unmanaged<IntPtr> WorkspaceGetWorkspaceDir;
    }
}