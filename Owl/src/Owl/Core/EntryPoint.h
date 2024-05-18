#pragma once

#include "Owl/Core/Application.h"
#include "Owl/Core/Base.h"
#include "Owl/Debug/Instrumentor.h"

extern Owl::Application* Owl::CreateApplication();

inline int main(int pArgc, char** pArgv)
{
	Owl::Log::Init();

	OWL_PROFILE_BEGIN_SESSION("Startup", "OwlProfile-Startup.json");
	const auto app = Owl::CreateApplication();
	OWL_PROFILE_END_SESSION();
	
	OWL_PROFILE_BEGIN_SESSION("Runtime", "OwlProfile-Runtime.json");
	app->Run();
	OWL_PROFILE_END_SESSION();
	
	OWL_PROFILE_BEGIN_SESSION("Shutdown", "OwlProfile-Shutdown.json");
	delete app;
	OWL_PROFILE_END_SESSION();
}
