using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ScriptingCore
{
    internal sealed class ScriptingCore
    {
        public static ScriptingCore Instance { get; } = new();
        
        static ScriptingCore()
        {
        }

        private ScriptingCore()
        {
            
        }

        public void Init()
        {
            
        }

        public void Update(double dt)
        {
            Console.WriteLine("ScriptingCore Update");
        }
    }
}
