#pragma once

#include "Base.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Owl
{
	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

template <typename OStream, glm::length_t L, typename T, glm::qualifier Q>
OStream& operator<<(OStream& pOs, const glm::vec<L, T, Q>& pVector)
{
	return pOs << glm::to_string(pVector);
}

template <typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
OStream& operator<<(OStream& pOs, const glm::mat<C, R, T, Q>& pMatrix)
{
	return pOs << glm::to_string(pMatrix);
}

template <typename OStream, typename T, glm::qualifier Q>
OStream& operator<<(OStream& pOs, glm::qua<T, Q> pQuaternion)
{
	return pOs << glm::to_string(pQuaternion);
}

// Core Log Macro
#define OWL_CORE_TRACE(...) ::Owl::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define OWL_CORE_INFO(...) ::Owl::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OWL_CORE_WARN(...) ::Owl::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OWL_CORE_ERROR(...) ::Owl::Log::GetCoreLogger()->error(__VA_ARGS__)
#define OWL_CORE_CRITICAL(...) ::Owl::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client Log Macro
#define OWL_TRACE(...) ::Owl::Log::GetClientLogger()->trace(__VA_ARGS__)
#define OWL_INFO(...) ::Owl::Log::GetClientLogger()->info(__VA_ARGS__)
#define OWL_WARN(...) ::Owl::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OWL_ERROR(...) ::Owl::Log::GetClientLogger()->error(__VA_ARGS__)
#define OWL_CRITICAL(...) ::Owl::Log::GetClientLogger()->critical(__VA_ARGS__)
