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
        [UnmanagedCallersOnly]
        public static void Init(IntPtr msg)
        {
            Console.WriteLine("Init C# Scripting Core...");
            var msgStr = Marshal.PtrToStringUTF8(msg);
            Console.WriteLine(msgStr);
        }
    }
}
