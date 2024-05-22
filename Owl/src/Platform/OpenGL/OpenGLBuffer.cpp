#include "opch.h"
#include "OpenGLBuffer.h"

#include "glad/glad.h"

namespace Owl
{
	// ========== VERTEX ===========
	OpenGlVertexBuffer::OpenGlVertexBuffer(uint32_t pSize)
	{
		OWL_PROFILE_FUNCTION();
		
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, pSize, nullptr, GL_DYNAMIC_DRAW);
	}
	
	OpenGlVertexBuffer::OpenGlVertexBuffer(const float* pVertices, const uint32_t pSize)
		: m_Layout()
	{
		OWL_PROFILE_FUNCTION();
		
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, pSize, pVertices, GL_STATIC_DRAW);
	}

	OpenGlVertexBuffer::~OpenGlVertexBuffer()
	{
		OWL_PROFILE_FUNCTION();
		
		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGlVertexBuffer::Bind() const
	{
		OWL_PROFILE_FUNCTION();
		
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGlVertexBuffer::UnBind() const
	{
		OWL_PROFILE_FUNCTION();
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGlVertexBuffer::SetData(const void* pData, const uint32_t pSize)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, pSize, pData);
	}

	// ========== INDEX ===========
	OpenGlIndexBuffer::OpenGlIndexBuffer(const uint32_t* pIndices, const uint32_t pCount)
		: m_Count(pCount)
	{
		OWL_PROFILE_FUNCTION();
		
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(pCount * sizeof(uint32_t)), pIndices,
		             GL_STATIC_DRAW);
	}

	OpenGlIndexBuffer::~OpenGlIndexBuffer()
	{
		OWL_PROFILE_FUNCTION();
		
		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGlIndexBuffer::Bind() const
	{
		OWL_PROFILE_FUNCTION();
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGlIndexBuffer::UnBind() const
	{
		OWL_PROFILE_FUNCTION();
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
