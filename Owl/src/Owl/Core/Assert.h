#pragma once

#include <filesystem>
#include "Owl/Core/Base.h"
#include "Owl/Core/Log.h"

#ifdef OWL_ENABLE_ASSERTS
#define OWL_ASSERT(x, ...) { if (!(x)) { OWL_ERROR("Assertion Failed '{0}' failed at {1}:{2}", __VA_ARGS__, std::filesystem::path(__FILE__).filename().string(), __LINE__); OWL_DEBUGBREAK(); }}
#define OWL_CORE_ASSERT(x, ...) { if (!(x)) { OWL_CORE_ERROR("Assertion Failed '{0}' failed at {1}:{2}", __VA_ARGS__, std::filesystem::path(__FILE__).filename().string(), __LINE__); OWL_DEBUGBREAK(); }}
#else
	#define OWL_ASSERT(...)
	#define OWL_CORE_ASSERT(...)
#endif
