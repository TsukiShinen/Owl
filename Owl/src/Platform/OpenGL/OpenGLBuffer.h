#pragma once
#include "Owl/Renderer/Buffer.h"

namespace Owl
{
	class OpenGlVertexBuffer : public VertexBuffer
	{
	public:
		OpenGlVertexBuffer(const float* pVertices, uint32_t pSize);
		~OpenGlVertexBuffer() override;
		
		void Bind() const override;
		void UnBind() const override;
		
		const BufferLayout& GetLayout() const override { return m_Layout; }
		void SetLayout(const BufferLayout& pLayout) override { m_Layout = pLayout; }
	private:
		uint32_t m_RendererId;
		BufferLayout m_Layout;
	};
	
	class OpenGlIndexBuffer : public IndexBuffer
	{
	public:
		OpenGlIndexBuffer(const uint32_t* pIndices, uint32_t pCount);
		~OpenGlIndexBuffer() override;
		
		void Bind() const override;
		void UnBind() const override;
		
		uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_RendererId;
		uint32_t m_Count;
	};
}
