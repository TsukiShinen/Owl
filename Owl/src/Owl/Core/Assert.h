#pragma once

#include <filesystem>
#include "Owl/Core/Base.h"
#include "Owl/Core/Log.h"

#ifdef OWL_ENABLE_ASSERTS
	#define OWL_ASSERT(x, ...) { if (!(x)) { OWL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
	#define OWL_CORE_ASSERT(x, ...) { if (!(x)) { OWL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
	#define OWL_ASSERT(...)
	#define OWL_CORE_ASSERT(...)
#endif