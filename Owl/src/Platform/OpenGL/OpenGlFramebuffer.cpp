﻿#include "opch.h"
#include "OpenGlFramebuffer.h"

#include <glad/glad.h>

namespace Owl
{
    static uint32_t s_MaxFrameBufferSize = 8192;
    
    OpenGlFramebuffer::OpenGlFramebuffer(const FramebufferSpecification& pSpecification)
        : m_Specification(pSpecification)
    {
        Invalidate();
    }

    OpenGlFramebuffer::~OpenGlFramebuffer()
    {
        glDeleteFramebuffers(1, &m_RendererId);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGlFramebuffer::Resize(uint32_t pWidth, uint32_t pHeight)
    {
        if (pWidth == 0 || pHeight == 0 || pWidth > s_MaxFrameBufferSize || pHeight > s_MaxFrameBufferSize)
        {
            OWL_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", pWidth, pHeight);
            return;
        }
        
        m_Specification.Width = pWidth;
        m_Specification.Height = pHeight;

        Invalidate();
    }

    void OpenGlFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }

    void OpenGlFramebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGlFramebuffer::Invalidate()
    {
        if (m_RendererId)
        {
            glDeleteFramebuffers(1, &m_RendererId);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteTextures(1, &m_DepthAttachment);
        }
        
        glCreateFramebuffers(1, &m_RendererId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

        OWL_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
