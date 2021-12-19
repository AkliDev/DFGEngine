#pragma once
#include <memory>

#include "DFGEngine/Core/PlatformDetection.h"

#ifdef DFG_DEBUG
	#if defined(DFG_PLATFORM_WINDOWS)
		#define DFG_DEBUGBREAK() __debugbreak()
	#elif defined(DFG_PLATFORM_LINUX)
		#include <signal.h>
		#define DFG_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define DFG_ENABLE_ASSERTS
#else
	#define DFG_DEBUGBREAK()
#endif

#define DFG_EXPAND_MACRO(x) x
#define DFG_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define DFG_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace DFGEngine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "DFGEngine/Core/Log.h"
#include "DFGEngine/Core/Assert.h"
