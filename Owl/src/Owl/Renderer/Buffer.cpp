#include "opch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Owl
{
	VertexBuffer* VertexBuffer::Create(float* pVertices, uint32_t pSize)
	{
		switch (Renderer::GetApi()) {
		case RendererApi::None:
			OWL_CORE_ASSERT(false, "RendererApi::None is currently not supported!")
			return nullptr;
		case RendererApi::OpenGL:
			return new OpenGlVertexBuffer(pVertices, pSize);
		}

		OWL_CORE_ASSERT(false, "Unknow RendererApi!")
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* pIndices, uint32_t pCount)
	{
		switch (Renderer::GetApi()) {
		case RendererApi::None:
			OWL_CORE_ASSERT(false, "RendererApi::None is currently not supported !")
			return nullptr;
		case RendererApi::OpenGL:
			return new OpenGlIndexBuffer(pIndices, pCount);
		}

		OWL_CORE_ASSERT(false, "Unknow RendererApi!")
		return nullptr;
	}
}
