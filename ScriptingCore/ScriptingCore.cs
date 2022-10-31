using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ScriptingInterface;

namespace ScriptingCore
{
    internal sealed class ScriptingCore
    {
        internal static ScriptingCore Instance { get; } = new();

        internal List<IUpdatable> Updatables { get; set; } = new ();
        internal List<IRenderable> Renderables { get; set; } = new ();
        internal List<ScriptBase> Scripts
        {
            get => _scripts;
            set
            {
                _scripts = value;
                foreach (var script in Scripts)
                {
                    script.Init();
                }
            }
        }

        private List<ScriptBase> _scripts = new ();

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
            foreach (var script in Scripts)
            {
                script.Update(dt);
            }
        }

        internal void Render(RenderContext context)
        {
            foreach (var script in Scripts)
            {
                script.Render(context);
            }
        }
    }
}
