#pragma once

#include <cstdint>
#include <string>

namespace RL
{
	class EventListener;
}

namespace RL::Interop
{
	enum struct LogLevel : int32_t {
		Info,
		Warning,
		Error,
		Fatal
	};
	
	class InteropService
	{
	public:
		void Init();
		void Shutdown();

		void Update(double dt);
		void Render();

		void BuildAssemblies();
		void LoadAssemblies();
		void UnloadAssemblies();
		void BuildAndLoadAssemblies();

	private:
		static void ScriptCoreLog(LogLevel Level, const char* Message);
		static void GraphicsLog(LogLevel Level, const char* Message);
		static void Exception(const char* Message);
		
		void* HostfxrLibrary {nullptr};
		EventListener* m_Listener {nullptr};
	};
}
