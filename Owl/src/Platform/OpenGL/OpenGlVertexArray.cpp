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
		OWL_PROFILE_FUNCTION();
		
		glCreateVertexArrays(1, &m_RendererId);
	}

	OpenGlVertexArray::~OpenGlVertexArray()
	{
		OWL_PROFILE_FUNCTION();
		
		glDeleteVertexArrays(1, &m_RendererId);
	}

	void OpenGlVertexArray::Bind() const
	{
		OWL_PROFILE_FUNCTION();
		
		glBindVertexArray(m_RendererId);
	}

	void OpenGlVertexArray::UnBind() const
	{
		OWL_PROFILE_FUNCTION();
		
		glBindVertexArray(0);
	}

	void OpenGlVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& pVertexBuffer)
	{
		OWL_PROFILE_FUNCTION();
		
		OWL_CORE_ASSERT(!pVertexBuffer->GetLayout().GetElements().empty(), "Vertex buffer has no layout!")

		glBindVertexArray(m_RendererId);
		pVertexBuffer->Bind();

		const auto& layout = pVertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGlBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)element.Offset);
					m_VertexBufferIndex++;
					break;
				}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribIPointer(m_VertexBufferIndex,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGlBaseType(element.Type),
						layout.GetStride(),
						(const void*)element.Offset);
					m_VertexBufferIndex++;
					break;
				}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
				{
					uint8_t count = element.GetComponentCount();
					for (uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(m_VertexBufferIndex);
						glVertexAttribPointer(m_VertexBufferIndex,
							count,
							ShaderDataTypeToOpenGlBaseType(element.Type),
							element.Normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(element.Offset + sizeof(float) * count * i));
						glVertexAttribDivisor(m_VertexBufferIndex, 1);
						m_VertexBufferIndex++;
					}
					break;
				}
			default:
				OWL_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}

		m_VertexBuffers.push_back(pVertexBuffer);
	}

	void OpenGlVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& pIndexBuffer)
	{
		OWL_PROFILE_FUNCTION();
		
		glBindVertexArray(m_RendererId);
		pIndexBuffer->Bind();

		m_IndexBuffers = pIndexBuffer;
	}
}
