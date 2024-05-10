#pragma once
#include "Owl/Renderer/VertexArray.h"

namespace Owl
{
	class OpenGlVertexArray : public VertexArray
	{
	public:
		OpenGlVertexArray();
		~OpenGlVertexArray() override;
		
		void Bind() const override;
		void UnBind() const override;
		
		void AddVertexBuffer(const Ref<VertexBuffer>& pVertexBuffer) override;
		void SetIndexBuffer(const Ref<IndexBuffer>& pIndexBuffer) override;
		
		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffers; }

	private:
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffers;

		uint32_t m_RendererId;
		uint32_t m_VertexBufferIndex = 0;
	};
	
}
