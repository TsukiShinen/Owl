#include "opch.h"
#include "Application.h"

#include "GLFW/glfw3.h"
#include "Owl/Renderer/Renderer.h"

namespace Owl
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		OWL_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(OWL_BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			const float time = static_cast<float>(glfwGetTime());
			const DeltaTime deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_IsMinimized)
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(deltaTime);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& pEvent)
	{
		EventDispatcher dispatcher(pEvent);
		dispatcher.Dispatch<WindowCloseEvent>(OWL_BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(OWL_BIND_EVENT_FN(OnWindowResize));

		for (auto iterator = m_LayerStack.end(); iterator != m_LayerStack.begin();)
		{
			(*--iterator)->OnEvent(pEvent);
			if (pEvent.IsHandled)
				break;
		}
	}

	void Application::PushLayer(Layer* pLayer)
	{
		m_LayerStack.PushLayer(pLayer);
	}

	void Application::PushOverlay(Layer* pOverlay)
	{
		m_LayerStack.PushOverlay(pOverlay);
	}

	bool Application::OnWindowClose(const WindowCloseEvent& pEvent)
	{
		m_IsRunning = false;
		return true;
	}

	bool Application::OnWindowResize(const WindowResizeEvent& pEvent)
	{
		if (pEvent.GetWidth() == 0 || pEvent.GetHeight() == 0)
		{
			m_IsMinimized = true;
			return false;
		}

		m_IsMinimized = false;
		Renderer::OnWindowResize(pEvent.GetWidth(), pEvent.GetHeight());

		return false;
	}
}
