#include "opch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGlTexture.h"

namespace Owl
{
	Ref<Texture2D> Texture2D::Create(uint32_t pWidth, uint32_t pHeight)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			OWL_CORE_ASSERT(false, "RendererApi::None is currently not supported!")
			return nullptr;
		case RendererApi::Api::OpenGl:
			return CreateRef<OpenGlTexture2D>(pWidth, pHeight);
		}

		OWL_CORE_ASSERT(false, "Unknow RendererApi!")
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& pPath)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			OWL_CORE_ASSERT(false, "RendererApi::None is currently not supported!")
			return nullptr;
		case RendererApi::Api::OpenGl:
			return CreateRef<OpenGlTexture2D>(pPath);
		}

		OWL_CORE_ASSERT(false, "Unknow RendererApi!")
		return nullptr;
	}
}
