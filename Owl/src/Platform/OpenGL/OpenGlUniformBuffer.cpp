#include "opch.h"
#include "OpenGlUniformBuffer.h"

#include "glad/glad.h"

namespace Owl
{
    OpenGLUniformBuffer::OpenGLUniformBuffer(const uint32_t pSize, const uint32_t pBinding)
    {
        glCreateBuffers(1, &m_RendererID);
        glNamedBufferData(m_RendererID, pSize, nullptr, GL_DYNAMIC_DRAW); // TODO: investigate usage hint
        glBindBufferBase(GL_UNIFORM_BUFFER, pBinding, m_RendererID);
    }
    
    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }
    
    void OpenGLUniformBuffer::SetData(const void* pData, const uint32_t pSize, const uint32_t pOffset)
    {
        glNamedBufferSubData(m_RendererID, pOffset, pSize, pData);
    }
}
