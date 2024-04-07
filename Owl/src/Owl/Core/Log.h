#pragma once

#include "Base.h"

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

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
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
