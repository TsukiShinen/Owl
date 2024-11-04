#include "opch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Owl
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t pSize)
	{
		switch (Renderer::GetApi())
		{
			case RendererApi::Api::None:
				OWL_CORE_ASSERT(false, "RendererApi::None is currently not supported!")
				return nullptr;
			case RendererApi::Api::OpenGl:
				return CreateRef<OpenGlVertexBuffer>(pSize);
		}

		OWL_CORE_ASSERT(false, "Unknow RendererApi!")
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(const float* pVertices, const uint32_t pSize)
	{
		switch (Renderer::GetApi())
		{
			case RendererApi::Api::None:
				OWL_CORE_ASSERT(false, "RendererApi::None is currently not supported!")
				return nullptr;
			case RendererApi::Api::OpenGl:
				return CreateRef<OpenGlVertexBuffer>(pVertices, pSize);
		}

		OWL_CORE_ASSERT(false, "Unknow RendererApi!")
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* pIndices, const uint32_t pCount)
	{
		switch (Renderer::GetApi())
		{
			case RendererApi::Api::None:
				OWL_CORE_ASSERT(false, "RendererApi::None is currently not supported !")
				return nullptr;
			case RendererApi::Api::OpenGl:
				return CreateRef<OpenGlIndexBuffer>(pIndices, pCount);
		}

		OWL_CORE_ASSERT(false, "Unknow RendererApi!")
		return nullptr;
	}
}
