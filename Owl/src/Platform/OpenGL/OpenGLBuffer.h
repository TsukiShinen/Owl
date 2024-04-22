#pragma once
#include "Owl/Renderer/Buffer.h"

namespace Owl
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const float* pVertices, uint32_t pSize);
		~OpenGLVertexBuffer() override;
		
		void Bind() const override;
		void UnBind() const override;
	private:
		uint32_t m_RendererId;
	};
	
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* pIndices, uint32_t pCount);
		~OpenGLIndexBuffer() override;
		
		void Bind() const override;
		void UnBind() const override;
		
		uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_RendererId;
		uint32_t m_Count;
	};
}
