#pragma once

#include "DFGEngine/Core/Base.h"
#include "DFGEngine/Core/Log.h"
#include <filesystem>

#ifdef DFG_ENABLE_ASSERTS
	// Alternatively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define DFG_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { DFG##type##ERROR(msg, __VA_ARGS__); DFG_DEBUGBREAK(); } }
	#define DFG_INTERNAL_ASSERT_WITH_MSG(type, check, ...) DFG_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define DFG_INTERNAL_ASSERT_NO_MSG(type, check) DFG_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", std::filesystem::path(__FILE__).filename().string(), __LINE__)
	
	#define DFG_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define DFG_INTERNAL_ASSERT_GET_MACRO(...) DFG_EXPAND_MACRO( DFG_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, DFG_INTERNAL_ASSERT_WITH_MSG, DFG_INTERNAL_ASSERT_NO_MSG) )
	
	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define DFG_ASSERT(...) DFG_EXPAND_MACRO( DFG_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define DFG_CORE_ASSERT(...) DFG_EXPAND_MACRO( DFG_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define DFG_ASSERT(...)
	#define DFG_CORE_ASSERT(...)
#endif
