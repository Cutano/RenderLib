#pragma once

#include <cstdint>
#include <string>

namespace RL
{
	class EventListener;
}

namespace RL::Interop
{
    enum struct StatusType : int32_t {
		Stopped,
		Idle,
		Running
	};

	enum struct TickState : int32_t {
		Stopped,
		Registered,
		Started
	};

	enum struct LogLevel : int32_t {
		Info,
		Warning,
		Error,
		Fatal
	};

	enum struct CallbackType : int32_t {
		SourceFileChangedDelegate
	};

	enum struct ArgumentType : int32_t {
		None,
		Single,
		Integer,
		Pointer,
		Callback
	};

	enum struct CommandType : int32_t {
		Initialize = 1,
		BuildAssemblies = 2,
		LoadAssemblies = 3,
		UnloadAssemblies = 4,
		BuildAndLoadAssemblies = 5,
		Find = 6,
		Execute = 7
	};

	enum {
		OnInit,
		OnUpdate,
		OnRender,
		OnSourceFileChanged
	};

	struct Callback {
		void** Parameters;
		CallbackType Type;

		FORCEINLINE Callback(void** Parameters, CallbackType Type) {
			this->Parameters = Parameters;
			this->Type = Type;
		}
	};

	struct Argument {
		union {
			double Single;
			uint32_t Integer;
			void* Pointer;
			Callback Callback;
		};
		ArgumentType Type;

		FORCEINLINE Argument(double Value) {
			this->Single = Value;
			this->Type = ArgumentType::Single;
		}

		FORCEINLINE Argument(uint32_t Value) {
			this->Integer = Value;
			this->Type = ArgumentType::Integer;
		}

		FORCEINLINE Argument(void* Value) {
			this->Pointer = Value;
			this->Type = !Value ? ArgumentType::None : ArgumentType::Pointer;
		}

		FORCEINLINE Argument(Interop::Callback Value) {
			this->Callback = Value;
			this->Type = ArgumentType::Callback;
		}
	};

	struct Command {
		union {
			struct {
				void* Buffer;
				int32_t Checksum;
			};
			struct {
				char* Method;
				int32_t Optional;
			};
			struct {
				void* Function;
				Argument Value;
			};
		};
		CommandType Type;

		FORCEINLINE Command(void* const Functions[3], int32_t Checksum) {
			this->Buffer = (void*)Functions;
			this->Checksum = Checksum;
			this->Type = CommandType::Initialize;
		}

		FORCEINLINE Command(CommandType Type) {
			this->Type = Type;
		}

		FORCEINLINE Command(const char* Method, bool Optional) {
			this->Method = const_cast<char*>(Method);
			this->Optional = Optional;
			this->Type = CommandType::Find;
		}

		FORCEINLINE Command(void* Function) {
			this->Function = Function;
			this->Value = nullptr;
			this->Type = CommandType::Execute;
		}

		FORCEINLINE Command(void* Function, Argument Value) {
			this->Function = Function;
			this->Value = Value;
			this->Type = CommandType::Execute;
		}
	};

	static_assert(sizeof(Callback) == 16, "Invalid size of the [Callback] structure");
	static_assert(sizeof(Argument) == 24, "Invalid size of the [Argument] structure");
	static_assert(sizeof(Command) == 40, "Invalid size of the [Command] structure");

	static void* (*ManagedCommand)(Command);

	static std::wstring ProjectPath;
	static std::wstring UserAssembliesPath;

	static StatusType Status = StatusType::Stopped;
	static TickState WorldTickState = TickState::Stopped;

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
		static void Log(LogLevel Level, const char* Message);
		static void Exception(const char* Message);
		
		void* HostfxrLibrary {nullptr};
		EventListener* m_Listener {nullptr};
	};

	namespace Shared
	{
		static constexpr int32_t storageSize = 128;

		// Non-instantiable

		static void* WorkspaceFunctions[storageSize];

		// Instantiable

		// Runtime
		
		static void* RuntimeFunctions[2];
		static void* Events[storageSize];
		static void* Functions[storageSize];
	}
	
	namespace Utility {
		FORCEINLINE static size_t Strcpy(char* Destination, const char* Source, size_t Length);
		FORCEINLINE static size_t Strlen(const char* Source);
	}
}
