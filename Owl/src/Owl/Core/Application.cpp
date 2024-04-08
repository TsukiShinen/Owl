#include "opch.h"
#include "Application.h"

#include <GLFW/glfw3.h>

namespace Owl
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(OWL_BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			glClearColor(.1f, .1f, .1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& pEvent)
	{
		EventDispatcher dispatcher(pEvent);
		dispatcher.Dispatch<WindowCloseEvent>(OWL_BIND_EVENT_FN(OnWindowClose));
	}

	bool Application::OnWindowClose(WindowCloseEvent& pCloseEvent)
	{
		m_IsRunning = false;
		return true;
	}
}
