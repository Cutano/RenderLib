#include "Log.h"

#include <spdlog/sinks/basic_file_sink.h>

#include <filesystem>
#include <vector>

namespace RL
{
	std::shared_ptr<spdlog::logger> Log::Logger()
	{
		static std::shared_ptr<spdlog::logger> s_Instance {nullptr};
		if (s_Instance)
		{
			return s_Instance;
		}

		// Create "logs" directory if doesn't exist
		if (const std::string logsDirectory = "logs"; !std::filesystem::exists(logsDirectory))
			std::filesystem::create_directories(logsDirectory);

		std::vector<spdlog::sink_ptr> sinks =
		{
			std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/RenderLib.log", true),
			std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>()
		};

		s_Instance = std::make_shared<spdlog::logger>("Render Lib", sinks.begin(), sinks.end());
		return s_Instance;
	}

	std::shared_ptr<spdlog::logger> Log::GraphicsLogger()
	{
		static std::shared_ptr<spdlog::logger> s_Instance {nullptr};
		if (s_Instance)
		{
			return s_Instance;
		}

		// Create "logs" directory if doesn't exist
		if (const std::string logsDirectory = "logs"; !std::filesystem::exists(logsDirectory))
			std::filesystem::create_directories(logsDirectory);

		std::vector<spdlog::sink_ptr> sinks =
		{
			std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/Graphics.log", true),
			std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>()
		};

		s_Instance = std::make_shared<spdlog::logger>("Graphics", sinks.begin(), sinks.end());
		return s_Instance;
	}
}
