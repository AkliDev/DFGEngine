#pragma once

#include "DFGEngine/Core/PlatformDetection.h"

//#ifdef DFG_PLATFORM_WINDOWS
//	#ifndef NOMINMAX
//		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
//		#define NOMINMAX
//	#endif
//#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <fstream>

#include "DFGEngine/Core/Base.h"

#include "DFGEngine/Core/Log.h"

//#include "DFEngine/Debug/Instrumentor.h"

#ifdef DFG_PLATFORM_WINDOWS
	#include <Windows.h>
	#ifdef DFG_RELEASE
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif
