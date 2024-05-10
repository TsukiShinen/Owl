#include "opch.h"
#include "OpenGlVertexArray.h"

#include "glad/glad.h"

namespace Owl
{
	static GLenum ShaderDataTypeToOpenGlBaseType(ShaderDataType pType)
	{
		switch (pType)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4: return GL_FLOAT;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4: return GL_INT;
		case ShaderDataType::Bool: return GL_BOOL;
		case ShaderDataType::None:
			OWL_CORE_ASSERT(false, "ShaderDataType::None is not Supported!")
			break;
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
		OWL_CORE_ASSERT(!pVertexBuffer->GetLayout().GetElements().empty(), "Vertex buffer has no layout!")
		
		glBindVertexArray(m_RendererId);
		pVertexBuffer->Bind();

		const auto& layout = pVertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex,
				static_cast<GLint>(element.GetComponentCount()),
				ShaderDataTypeToOpenGlBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				static_cast<GLsizei>(layout.GetStride()),
				(const void*)(uintptr_t)element.Offset);
			m_VertexBufferIndex++;
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
