using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Build.Locator;

namespace ScriptingCore
{
    public static partial class Entry
    {
        [UnmanagedCallersOnly]
        // ReSharper disable once UnusedMember.Global
        public static unsafe ManagedFunctionPayload Init(UnmanagedFunctionPayload unmanagedPayload)
        {
            Console.WriteLine("Init C# Scripting Core...");

            MSBuildLocator.RegisterDefaults();
            
            Workspace.Instance.Init(unmanagedPayload);
            ScriptingCore.Instance.Init();

            ManagedFunctionPayload managedPayload;
            managedPayload.UpdateManaged = &Update;
            return managedPayload;
        }

        [UnmanagedCallersOnly]
        public static void Update(double dt)
        {
            ScriptingCore.Instance.Update(dt);
        }
    }
}
