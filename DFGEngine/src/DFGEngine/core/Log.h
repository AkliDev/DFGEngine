#pragma once

#include "Base.h"
// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include <spdlog/fmt/bundled/printf.h>
#pragma warning(pop)

namespace DFGEngine
{
	class Log
	{
	public: 
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		inline static Ref<spdlog::logger>& GetNetowrkLogger() { return s_NetworkLogger; }

		inline static Ref<spdlog::logger>& GetSyncLogger() { return s_SyncLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
		static Ref<spdlog::logger> s_NetworkLogger;

		static Ref<spdlog::logger> s_SyncLogger;
	};
}

//Core log macros
#define DFG_CORE_TRACE(...)		::DFGEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DFG_CORE_WARN(...)		::DFGEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DFG_CORE_INFO(...)		::DFGEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DFG_CORE_ERROR(...)		::DFGEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DFG_CORE_CRITICAL(...)	::DFGEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client log macros
#define DFG_TRACE(...)			::DFGEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DFG_WARN(...)			::DFGEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DFG_INFO(...)			::DFGEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define DFG_ERROR(...)			::DFGEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define DFG_CRITICAL(...)		::DFGEngine::Log::GetClientLogger()->critical(__VA_ARGS__)

//Client Sync log macros
#define DFG_SYNC_TRACE(...)		::DFGEngine::Log::GetSyncLogger()->trace(__VA_ARGS__)
#define DFG_SYNC_WARN(...)		::DFGEngine::Log::GetSyncLogger()->warn(__VA_ARGS__)
#define DFG_SYNC_INFO(...)		::DFGEngine::Log::GetSyncLogger()->info(__VA_ARGS__)
#define DFG_SYNC_ERROR(...)		::DFGEngine::Log::GetSyncLogger()->error(__VA_ARGS__)
#define DFG_SYNC_CRITICAL(...)	::DFGEngine::Log::GetSyncLogger()->critical(__VA_ARGS__)



template <class loggerPtr, class... Args>
void loglineprintf(loggerPtr                 logger,
	spdlog::level::level_enum level,
	spdlog::source_loc        loc,
	const char* fmt,
	const Args&... args) noexcept
{
	if (logger && logger->should_log(level))
	{
		logger->log(loc, level, "{}", fmt::sprintf(fmt, args...));
	}
}

#define SPDLOG_LOGGER_PRINTF(logger, level, ...) \
    loglineprintf(logger, level, spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, __VA_ARGS__)

//specific log implementation macros
#define NBN_LogTrace(...)	SPDLOG_LOGGER_PRINTF(spdlog::default_logger(),spdlog::level::trace,__VA_ARGS__)
#define NBN_LogWarning(...)	SPDLOG_LOGGER_PRINTF(spdlog::default_logger(),spdlog::level::warn,__VA_ARGS__)
#define NBN_LogInfo(...)	SPDLOG_LOGGER_PRINTF(spdlog::default_logger(),spdlog::level::info,__VA_ARGS__)
#define NBN_LogError(...)	SPDLOG_LOGGER_PRINTF(spdlog::default_logger(),spdlog::level::err,__VA_ARGS__)
#define NBN_LogDebug(...)	SPDLOG_LOGGER_PRINTF(spdlog::default_logger(),spdlog::level::info,__VA_ARGS__)



		
		

