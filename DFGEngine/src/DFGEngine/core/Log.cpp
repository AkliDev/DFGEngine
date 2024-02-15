#include "dfgpch.h"
#include "DFGEngine/Core/Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/sinks/basic_file_sink.h>

namespace DFGEngine
{
	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;
	Ref<spdlog::logger> Log::s_NetworkLogger;

	Ref<spdlog::logger> Log::s_SyncLogger;

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("DFGEngine.log", true));
		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		std::vector<spdlog::sink_ptr> syncLogSinks;
		syncLogSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		syncLogSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Sync.log", true));
		syncLogSinks[0]->set_pattern("%^%n: %v%$");
		syncLogSinks[1]->set_pattern("[%l] %n: %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("DFGEngine", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);

		s_NetworkLogger = std::make_shared<spdlog::logger>("Network", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_NetworkLogger);
		s_NetworkLogger->set_level(spdlog::level::trace);
		s_NetworkLogger->flush_on(spdlog::level::trace);


		s_SyncLogger = std::make_shared<spdlog::logger>("Sync", begin(syncLogSinks), end(syncLogSinks));
		spdlog::register_logger(s_SyncLogger);
		s_SyncLogger->set_level(spdlog::level::trace);
		s_SyncLogger->flush_on(spdlog::level::trace);
	}
}