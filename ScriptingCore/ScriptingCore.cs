using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ScriptingCore
{
    internal sealed class ScriptingCore
    {
        internal static ScriptingCore Instance { get; } = new();
        
        static ScriptingCore()
        {
        }

        private ScriptingCore()
        {
            
        }

        internal void Init()
        {
            
        }

        internal void Update(double dt)
        {
            // Console.WriteLine("ScriptingCore Update");
        }
    }
}
