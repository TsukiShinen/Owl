#pragma once

#include "Base.h"
#include "LayerStack.h"
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

		void PushLayer(Layer* pLayer);
		void PushOverlay(Layer* pOverlay);
	private:
		bool OnWindowClose(WindowCloseEvent& pCloseEvent);
		
		Scope<Window> m_Window;
		bool m_IsRunning = true;
		LayerStack m_LayerStack;
	};

	// To be defined in client
	Application* CreateApplication();
}
