#pragma once

#include "Base.h"
#include "Window.h"
#include "Owl/Events/ApplicationEvent.h"

namespace Owl
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& pEvent);
	private:
		bool OnWindowClose(WindowCloseEvent& pCloseEvent);
		
		Scope<Window> m_Window;
		bool m_IsRunning = true;
	};

	// To be defined in client
	Application* CreateApplication();
}
