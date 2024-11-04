#include "opch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGlFramebuffer.h"

namespace Owl
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& pSpecification)
	{
		switch (Renderer::GetApi())
		{
			case RendererApi::Api::None:
				OWL_CORE_ASSERT(false, "RendererAPI::NONE is currently not supported!");
				return nullptr;
			case RendererApi::Api::OpenGl:
				return CreateRef<OpenGlFramebuffer>(pSpecification);
		}

		OWL_CORE_ASSERT(false, "Unknow RendererAPI!")
		return nullptr;
	}
}
