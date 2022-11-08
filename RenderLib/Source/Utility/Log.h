#pragma once

#include <spdlog/spdlog.h>

#include <memory>

namespace RL
{
	class Log
	{
	public:
		static std::shared_ptr<spdlog::logger> Logger();
		static std::shared_ptr<spdlog::logger> GraphicsEngineLogger();
		static std::shared_ptr<spdlog::logger> ScriptCoreLogger();
		static std::shared_ptr<spdlog::logger> GraphicsLogger();
	};
}