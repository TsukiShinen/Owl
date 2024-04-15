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
	}

	void OpenGLContext::SwapBuffer()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}
