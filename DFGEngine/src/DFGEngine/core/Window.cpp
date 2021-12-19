#include "dfgpch.h"
#include "Window.h"

#ifdef DFG_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace DFGEngine
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef DFG_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		DFG_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}
