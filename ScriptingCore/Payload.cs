using System.Runtime.InteropServices;

namespace ScriptingCore;

partial class Entry
{
    [StructLayout(LayoutKind.Sequential)]
    public unsafe struct ManagedFunctionPayload
    {
        public delegate* unmanaged<double, void> UpdateManaged;
    }

    [StructLayout(LayoutKind.Sequential)]
    public unsafe struct UnmanagedFunctionPayload
    {
        public delegate* unmanaged<IntPtr> WorkspaceGetAppPath;
        public delegate* unmanaged<IntPtr> WorkspaceGetWorkspaceDir;
    }
}