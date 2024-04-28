#pragma once
#include "Owl/Renderer/RendererApi.h"

namespace Owl
{
	class OpenGlRendererApi : public RendererApi
	{
	public:
		void SetClearColor(const glm::vec4& pColor) override;
		void Clear() override;
		
		void DrawIndexed(const Ref<VertexArray>& pVertexArray) override;
	};
	
}
