#include "opch.h"
#include "Input.h"

#ifdef OWL_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsInput.h"
#endif

namespace Owl {

	Scope<Input> Input::s_Instance = Create();

	Scope<Input> Input::Create()
	{
#ifdef OWL_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
#else
		OWL_CORE_ASSERT(false, "Unknown platform!")
		return nullptr;
#endif
	}
} 