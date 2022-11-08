using System.Runtime.InteropServices;

namespace ScriptingCore;

public static unsafe class Log
{
    enum LogLevel
    {
        Info,
        Warning,
        Error,
        Fatal
    }
    
    internal static delegate* unmanaged<int, IntPtr, void> LogUnmanaged;

    public static void Info(string msg)
    {
        var ptr = Marshal.StringToHGlobalAnsi(msg);
        LogUnmanaged((int)LogLevel.Info, ptr);
        Marshal.FreeHGlobal(ptr);
    }
    
    public static void Warning(string msg)
    {
        var ptr = Marshal.StringToHGlobalAnsi(msg);
        LogUnmanaged((int)LogLevel.Warning, ptr);
        Marshal.FreeHGlobal(ptr);
    }
    
    public static void Error(string msg)
    {
        var ptr = Marshal.StringToHGlobalAnsi(msg);
        LogUnmanaged((int)LogLevel.Error, ptr);
        Marshal.FreeHGlobal(ptr);
    }
    
    public static void Fatal(string msg)
    {
        var ptr = Marshal.StringToHGlobalAnsi(msg);
        LogUnmanaged((int)LogLevel.Fatal, ptr);
        Marshal.FreeHGlobal(ptr);
    }
}