using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Build.Locator;
using Serilog;

namespace ScriptingCore
{
    internal sealed class AssembliesContextManager {
        internal ScriptLoadContext? ScriptLoadContext;

        [MethodImpl(MethodImplOptions.NoInlining)]
        internal WeakReference CreateAssembliesContext(string path) {
            ScriptLoadContext = new ScriptLoadContext(path);

            return new WeakReference(ScriptLoadContext, trackResurrection: true);
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        internal void UnloadAssembliesContext() => ScriptLoadContext?.Unload();
    }
    
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
            
            Workspace.Instance.Init(unmanagedPayload);
            ScriptingCore.Instance.Init();

            ManagedFunctionPayload managedPayload;
            managedPayload.UpdateManaged = &Update;
            managedPayload.OnCsharpFileChangedManaged = &Workspace.OnCsharpFileChanged;
            managedPayload.RecompileAssemblyManaged = &Workspace.RecompileAssembly;
            managedPayload.ReloadAssemblyManaged = &Workspace.ReloadAssembly;
            return managedPayload;
        }

        [UnmanagedCallersOnly]
        internal static void Update(double dt)
        {
            ScriptingCore.Instance.Update(dt);
        }
    }
}
