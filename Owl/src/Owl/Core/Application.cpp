#include "opch.h"
#include "Application.h"

#include <ranges>

#include "GLFW/glfw3.h"
#include "Owl/Renderer/Renderer.h"

namespace Owl
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& pName, const ApplicationCommandLineArgs pArgs)
		: m_CommandLineArgs(pArgs)
	{
		OWL_PROFILE_FUNCTION();

		OWL_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(pName));
		m_Window->SetEventCallback(OWL_BIND_EVENT_FN(OnEvent));
		m_Window->SetVSync(false);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		OWL_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::Run()
	{
		OWL_PROFILE_FUNCTION();

		while (m_IsRunning)
		{
			OWL_PROFILE_SCOPE("RunLoop");
			const auto time = static_cast<float>(glfwGetTime());
			const DeltaTime deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_IsMinimized)
			{
				OWL_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(deltaTime);
			}

			m_ImGuiLayer->Begin();
			{
				OWL_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& pEvent)
	{
		OWL_PROFILE_FUNCTION();

		EventDispatcher dispatcher(pEvent);
		dispatcher.Dispatch<WindowCloseEvent>(OWL_BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(OWL_BIND_EVENT_FN(OnWindowResize));

		for (auto& iterator : std::ranges::reverse_view(m_LayerStack))
		{
			if (pEvent.IsHandled)
				break;
			iterator->OnEvent(pEvent);
		}
	}

	void Application::PushLayer(Layer* pLayer)
	{
		OWL_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(pLayer);
		pLayer->OnAttach();
	}

	void Application::PushOverlay(Layer* pOverlay)
	{
		OWL_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(pOverlay);
		pOverlay->OnAttach();
	}

	void Application::Close()
	{
		m_IsRunning = false;
	}

	bool Application::OnWindowClose(const WindowCloseEvent& pEvent)
	{
		m_IsRunning = false;
		return true;
	}

	bool Application::OnWindowResize(const WindowResizeEvent& pEvent)
	{
		OWL_PROFILE_FUNCTION();

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
