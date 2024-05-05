#include "opch.h"
#include "Renderer.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGlShader.h"

namespace Owl
{
	Renderer::SceneData* Renderer::m_SceneData = new SceneData();
	
	void Renderer::BeginScene(const OrthographicCamera& pCamera)
	{
		m_SceneData->ViewProjectionMatrix = pCamera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& pVertexArray, const Ref<Shader>& pShader, const glm::mat4& pTransform)
	{
		pShader->Bind();
		std::dynamic_pointer_cast<OpenGlShader>(pShader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGlShader>(pShader)->UploadUniformMat4("u_Transform", pTransform);
		
		pVertexArray->Bind();
		RenderCommand::DrawIndexed(pVertexArray);
	}
}
