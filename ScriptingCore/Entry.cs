using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ScriptingCore
{
    public static class Entry
    {
        public unsafe struct ManagedFunctionPayload
        {
            public delegate* unmanaged<double, void> UpdateManaged;
        }
        
        [UnmanagedCallersOnly]
        public static unsafe ManagedFunctionPayload Init(IntPtr msg)
        {
            Console.WriteLine("Init C# Scripting Core...");
            var msgStr = Marshal.PtrToStringUni(msg);
            Console.WriteLine(msgStr);

            ScriptingCore.Instance.Init();

            ManagedFunctionPayload payload;
            payload.UpdateManaged = &Update;
            return payload;
        }

        [UnmanagedCallersOnly]
        public static void Update(double dt)
        {
            ScriptingCore.Instance.Update(dt);
        }
    }
}
