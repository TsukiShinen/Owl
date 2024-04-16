#include "opch.h"
#include "OpenGLContext.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Owl
{
	OpenGLContext::OpenGLContext(GLFWwindow* pWindowHandle)
		: m_WindowHandle(pWindowHandle)
	{
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		OWL_CORE_ASSERT(status, "Failed to initialize Glad!")

		OWL_CORE_INFO("OpenGL Info :");
		OWL_CORE_INFO("-- Vendor : {0}", (const char*)glGetString(GL_VENDOR));
		OWL_CORE_INFO("-- Renderer : {0}", (const char*)glGetString(GL_RENDERER));
		OWL_CORE_INFO("-- Version : {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffer()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}
