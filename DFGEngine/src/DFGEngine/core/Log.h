#pragma once

#include "Base.h"
// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

namespace DFGEngine
{
	class Log
	{
	public: 
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
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