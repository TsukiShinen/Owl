#pragma once
#include "Owl/Renderer/FrameBuffer.h"

namespace Owl
{
    class OpenGlFramebuffer : public Framebuffer
    {
    public:
        OpenGlFramebuffer(const FramebufferSpecification& pSpecification);
        ~OpenGlFramebuffer() override;
        
        void Bind() override;
        void Unbind() override;
        uint32_t GetColorAttachmentRendererId() const override {return m_ColorAttachment; }
        const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

        void Invalidate();

    private:
        uint32_t m_RendererId;
        uint32_t m_ColorAttachment, m_DepthAttachment;
        FramebufferSpecification m_Specification;
    };
}
