#include "opch.h"
#include "Renderer.h"

#include "RenderCommand.h"

namespace Owl
{
	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& pVertexArray)
	{
		pVertexArray->Bind();
		RenderCommand::DrawIndexed(pVertexArray);
	}
}
