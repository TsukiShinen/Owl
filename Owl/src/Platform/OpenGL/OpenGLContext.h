#pragma once
#include "Owl/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Owl
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* pWindowHandle);

		void Init() override;
		void SwapBuffer() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}
