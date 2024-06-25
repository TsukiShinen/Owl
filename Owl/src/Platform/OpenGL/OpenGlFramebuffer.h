#pragma once
#include "Owl/Renderer/FrameBuffer.h"

namespace Owl
{
    class OpenGlFramebuffer : public Framebuffer
    {
    public:
        OpenGlFramebuffer(const FramebufferSpecification& pSpecification);
        ~OpenGlFramebuffer() override;
        
        void Resize(uint32_t pWidth, uint32_t pHeight) override;
        
        void Bind() override;
        void Unbind() override;
        uint32_t GetColorAttachmentRendererId() const override {return m_ColorAttachment; }
        const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

        void Invalidate();

    private:
        uint32_t m_RendererId = 0;
        uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
        FramebufferSpecification m_Specification;
    };
}
