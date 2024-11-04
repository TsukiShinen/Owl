#include "opch.h"
#include "UniformBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGlUniformBuffer.h"

namespace Owl
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t pSize, uint32_t pBinding)
	{
		switch (Renderer::GetApi())
		{
			case RendererApi::Api::None: OWL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
				return nullptr;
			case RendererApi::Api::OpenGl: return CreateRef<OpenGLUniformBuffer>(pSize, pBinding);
		}
		OWL_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}
}
