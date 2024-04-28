#include "opch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGlVertexArray.h"

namespace Owl
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetApi()) {
		case RendererApi::Api::None:
			OWL_CORE_ASSERT(false, "RendererApi::None is currently not supported!")
			return nullptr;
		case RendererApi::Api::OpenGl:
			return new OpenGlVertexArray();
		}

		OWL_CORE_ASSERT(false, "Unknow RendererApi!")
		return nullptr;
	}
}
