using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Build.Locator;
using Serilog;

namespace ScriptingCore
{
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
            
            Console.WriteLine("Init C# Scripting Core...");

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
