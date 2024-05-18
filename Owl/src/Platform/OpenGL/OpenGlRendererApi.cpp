#include "opch.h"
#include "OpenGlRendererApi.h"

#include "glad/glad.h"

namespace Owl
{
	void OpenGlMessageCallback(
			unsigned pSource,
			unsigned pType,
			unsigned pId,
			unsigned pSeverity,
			int pLength,
			const char* pMessage,
			const void* pUserParam)
	{
		switch (pSeverity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         OWL_CORE_CRITICAL(pMessage); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       OWL_CORE_ERROR(pMessage); return;
		case GL_DEBUG_SEVERITY_LOW:          OWL_CORE_WARN(pMessage); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: OWL_CORE_TRACE(pMessage); return;
		default: ;
		}

		OWL_CORE_ASSERT(false, "Unknown severity level!")
	}
	
	void OpenGlRendererApi::Init()
	{
		OWL_PROFILE_FUNCTION();
		
#ifdef OWL_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGlMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGlRendererApi::SetViewport(const int pX, const int pY, const uint32_t pWidth, const uint32_t pHeight)
	{
		glViewport(pX, pY, static_cast<GLsizei>(pWidth), static_cast<GLsizei>(pHeight));
	}

	void OpenGlRendererApi::SetClearColor(const glm::vec4& pColor)
	{
		glClearColor(pColor.r, pColor.g, pColor.b, pColor.a);
	}

	void OpenGlRendererApi::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGlRendererApi::DrawIndexed(const Ref<VertexArray>& pVertexArray)
	{
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(pVertexArray->GetIndexBuffer()->GetCount()), GL_UNSIGNED_INT,
		               nullptr);
	}
}
