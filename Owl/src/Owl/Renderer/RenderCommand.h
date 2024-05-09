#pragma once
#include "RendererApi.h"

namespace Owl
{
	class RenderCommand
	{
	public:
		static void Init()
		{
			s_RendererApi->Init();
		}

		static void SetViewport(const int pX, const int pY, const uint32_t pWidth, const uint32_t pHeight)
		{
			s_RendererApi->SetViewport(pX, pY, pWidth, pHeight);
		}
		
		static void SetClearColor(const glm::vec4& pColor)
		{
			s_RendererApi->SetClearColor(pColor);
		}
		
		static void Clear()
		{
			s_RendererApi->Clear();
		}
		
		static void DrawIndexed(const Ref<VertexArray>& pVertexArray)
		{
			s_RendererApi->DrawIndexed(pVertexArray);
		}

	private:
		static RendererApi* s_RendererApi;
	};
	
}
