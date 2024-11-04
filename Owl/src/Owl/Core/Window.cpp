#include "opch.h"
#include "Window.h"

#ifdef OWL_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Owl
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
		#ifdef OWL_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
		#else
		OWL_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
		#endif
	}
}
