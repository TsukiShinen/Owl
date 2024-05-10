#pragma once

#include "Owl/Core/Base.h"
#include "Owl/Core/LayerStack.h"
#include "Owl/Core/Window.h"
#include "Owl/Events/ApplicationEvent.h"
#include "Owl/ImGui/ImGuiLayer.h"

namespace Owl
{
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& pEvent);

		void PushLayer(Layer* pLayer);
		void PushOverlay(Layer* pOverlay);

		Window& GetWindow() const { return *m_Window; }

		static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(const WindowCloseEvent& pEvent);
		bool OnWindowResize(const WindowResizeEvent& pEvent);
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_IsRunning = true;
		bool m_IsMinimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();
}
