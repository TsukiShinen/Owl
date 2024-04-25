#include "opch.h"
#include "OpenGlVertexArray.h"

#include "glad/glad.h"

namespace Owl
{
	static GLenum ShaderDataTypeToOpenGlBaseType(ShaderDataType pType)
	{
		switch (pType)
		{
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Mat3: return GL_FLOAT;
		case ShaderDataType::Mat4: return GL_FLOAT;
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Int2: return GL_INT;
		case ShaderDataType::Int3: return GL_INT;
		case ShaderDataType::Int4: return GL_INT;
		case ShaderDataType::Bool: return GL_BOOL;
		}

		OWL_CORE_ASSERT(false, "Unknown ShaderDataType!")
		return 0;
	}
	
	OpenGlVertexArray::OpenGlVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererId);
	}

	OpenGlVertexArray::~OpenGlVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererId);
	}

	void OpenGlVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererId);
	}

	void OpenGlVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGlVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& pVertexBuffer)
	{
		OWL_CORE_ASSERT(pVertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!")
		
		glBindVertexArray(m_RendererId);
		pVertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = pVertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGlBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				reinterpret_cast<const void*>(element.Offset));
			index++;
		}

		m_VertexBuffers.push_back(pVertexBuffer);
	}

	void OpenGlVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& pIndexBuffer)
	{
		glBindVertexArray(m_RendererId);
		pIndexBuffer->Bind();

		m_IndexBuffers = pIndexBuffer;
	}
}
