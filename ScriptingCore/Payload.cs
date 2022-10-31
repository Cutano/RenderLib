﻿using System.Runtime.InteropServices;

namespace ScriptingCore;

partial class Entry
{
    [StructLayout(LayoutKind.Sequential)]
    internal unsafe struct ManagedFunctionPayload
    {
        internal delegate* unmanaged<double, void> UpdateManaged;
        internal delegate* unmanaged<Workspace.FileAction, IntPtr, void> OnCsharpFileChangedManaged;
        internal delegate* unmanaged<void> RecompileAssemblyManaged;
        internal delegate* unmanaged<void> ReloadAssemblyManaged;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal unsafe struct UnmanagedFunctionPayload
    {
        internal delegate* unmanaged<IntPtr> WorkspaceGetAppPath;
        internal delegate* unmanaged<IntPtr> WorkspaceGetWorkspaceDir;
    }
}