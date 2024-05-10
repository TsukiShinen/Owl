#pragma once
#include "OrthographicCamera.h"
#include "RendererApi.h"
#include "Shader.h"

namespace Owl
{
	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t pWidth, uint32_t pHeight);
		
		static void BeginScene(const OrthographicCamera& pCamera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& pVertexArray, const Ref<Shader>& pShader, const glm::mat4& pTransform = glm::mat4(1.0f));
		
		static RendererApi::Api GetApi() { return RendererApi::GetApi(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};
	
}
