#pragma once

#include "Owl/Core/Application.h"
#include "Owl/Core/Base.h"

extern Owl::Application* Owl::CreateApplication();

inline int main(int pArgc, char** pArgv)
{
	Owl::Log::Init();

	const auto app = Owl::CreateApplication();
	app->Run();
	delete app;
}
