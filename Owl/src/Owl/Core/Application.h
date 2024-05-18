#pragma once

#include "Owl/Core/Base.h"
#include "Owl/Core/LayerStack.h"
#include "Owl/Core/Window.h"
#include "Owl/Events/ApplicationEvent.h"
#include "Owl/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Owl
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& pEvent);

		void PushLayer(Layer* pLayer);
		void PushOverlay(Layer* pOverlay);

		Window& GetWindow() const { return *m_Window; }

		static Application& Get() { return *s_Instance; }

	private:
		void Run();
		
		bool OnWindowClose(const WindowCloseEvent& pEvent);
		bool OnWindowResize(const WindowResizeEvent& pEvent);
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_IsRunning = true;
		bool m_IsMinimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in client
	Application* CreateApplication();
}
