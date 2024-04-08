﻿#include "opch.h"
#include "WindowsWindow.h"

#include "Owl/Events/ApplicationEvent.h"
#include "Owl/Events/KeyEvent.h"
#include "Owl/Events/MouseEvent.h"

namespace Owl
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int pError, const char* pDescription)
	{
		OWL_CORE_ERROR("GLFW Error ({0}) : {1}", pError, pDescription);
	}
	
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
			OWL_CORE_ASSERT(success, "Could not initialize GLFW!")
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow(static_cast<int>(m_Data.Width), static_cast<int>(m_Data.Height), m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* pWindow, int pWidth, int pHeight)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
			data.Width = pWidth;
			data.Height = pHeight;

			WindowResizeEvent event(pWidth, pHeight);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* pWindow)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* pWindow, int pKey, int pScanCode, int pAction, int pMods)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

			switch (pAction)
			{
			case GLFW_PRESS:
				{
					KeyPressedEvent event(pKey, 0);
					data.EventCallback(event);
					break;
				}
			case GLFW_RELEASE:
				{
					KeyReleasedEvent event(pKey);
					data.EventCallback(event);
					break;
				}
			case GLFW_REPEAT:
				{
					KeyPressedEvent event(pKey, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* pWindow, int pButton, int pAction, int pMods)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

			switch (pAction)
			{
			case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(pButton);
					data.EventCallback(event);
					break;
				}
			case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(pButton);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* pWindow, double pXOffset, double pYOffset)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

			MouseScrolledEvent event(static_cast<float>(pXOffset), static_cast<float>(pYOffset));
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* pWindow, double pXPosition, double pYPosition)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

			MouseMovedEvent event(static_cast<float>(pXPosition), static_cast<float>(pYPosition));
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}