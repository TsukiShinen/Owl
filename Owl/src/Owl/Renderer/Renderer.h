#pragma once
#include "OrthographicCamera.h"
#include "RendererApi.h"
#include "Shader.h"

namespace Owl
{
	class Renderer
	{
	public:
		static void BeginScene(const OrthographicCamera& pCamera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& pVertexArray, const Ref<Shader>& pShader, const glm::mat4& pTransform = glm::mat4(1.0f));
		
		static RendererApi::Api GetApi() { return RendererApi::GetApi(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
	
}
