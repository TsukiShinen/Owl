#pragma once
#include "Owl/Renderer/RendererApi.h"

namespace Owl
{
	class OpenGlRendererApi : public RendererApi
	{
	public:
		void Init() override;
		void SetViewport(int pX, int pY, uint32_t pWidth, uint32_t pHeight) override;
		
		void SetClearColor(const glm::vec4& pColor) override;
		void Clear() override;
		
		void DrawIndexed(const Ref<VertexArray>& pVertexArray) override;
	};
	
}
