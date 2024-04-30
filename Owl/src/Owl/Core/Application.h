#pragma once

#include "Base.h"
#include "LayerStack.h"
#include "Window.h"
#include "Owl/Events/ApplicationEvent.h"
#include "Owl/ImGui/ImGuiLayer.h"

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
		
		Window& GetWindow() const { return *m_Window; }

		static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& pCloseEvent);
		
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_IsRunning = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();
}
