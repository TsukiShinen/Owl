#include "opch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGlVertexArray.h"

namespace Owl
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			OWL_CORE_ASSERT(false, "RendererApi::None is currently not supported!")
			return nullptr;
		case RendererApi::Api::OpenGl:
			return CreateRef<OpenGlVertexArray>();
		}

		OWL_CORE_ASSERT(false, "Unknow RendererApi!")
		return nullptr;
	}
}
