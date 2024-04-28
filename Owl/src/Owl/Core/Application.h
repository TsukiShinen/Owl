#pragma once

#include "Base.h"
#include "LayerStack.h"
#include "Window.h"
#include "Owl/Events/ApplicationEvent.h"
#include "Owl/ImGui/ImGuiLayer.h"
#include "Owl/Renderer/Shader.h"
#include "Owl/Renderer/VertexArray.h"

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

		Ref<VertexArray> m_TriangleVertexArray;
		Ref<Shader> m_Shader;
		
		Ref<VertexArray> m_SquareVertexArray;
		Ref<Shader> m_BlueShader;
	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();
}
