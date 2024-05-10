#include "opch.h"
#include "OpenGlRendererApi.h"

#include "glad/glad.h"

namespace Owl
{
	void OpenGlRendererApi::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(pVertexArray->GetIndexBuffer()->GetCount()), GL_UNSIGNED_INT, nullptr);
	}
}
