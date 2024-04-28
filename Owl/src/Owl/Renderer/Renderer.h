#pragma once
#include "RendererApi.h"

namespace Owl
{
	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const Ref<VertexArray>& pVertexArray);
		
		static RendererApi::Api GetApi() { return RendererApi::GetApi(); }
	};
	
}
