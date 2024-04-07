#include "opch.h"
#include "WindowsWindow.h"

namespace Owl
{
	static bool s_GLFWInitialized = false;
	
	Window* Window::Create(const WindowProps& pProps)
	{
		return new WindowsWindow(pProps);
	}

	WindowsWindow::WindowsWindow(const WindowProps& pProps)
	{
		Init(pProps);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(const bool pEnabled)
	{
		if (pEnabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.IsVSync = pEnabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.IsVSync;
	}

	void WindowsWindow::Init(const WindowProps& pProps)
	{
		m_Data.Title = pProps.Title;
		m_Data.Width = pProps.Width;
		m_Data.Height = pProps.Height;

		OWL_CORE_INFO("Creating window {0} ({1}, {2})", m_Data.Title, m_Data.Width, m_Data.Height);

		if (!s_GLFWInitialized)
		{
			const int success = glfwInit();
			OWL_CORE_ASSERT(success, "Could not initialize GLFW!");
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow(static_cast<int>(m_Data.Width), static_cast<int>(m_Data.Height), m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}
