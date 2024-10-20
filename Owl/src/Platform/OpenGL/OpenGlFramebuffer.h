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
        int ReadPixel(uint32_t pAttachmentIndex, int pX, int pY) override;
        
        void ClearAttachment(uint32_t pAttachmentIndex, int pValue) override;
        
        void Bind() override;
        void Unbind() override;
        uint32_t GetColorAttachmentRendererId(uint32_t pIndex = 0) const override { OWL_CORE_ASSERT(pIndex < m_ColorAttachments.size()) return m_ColorAttachments[pIndex]; }
        const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

        void Invalidate();

    private:
        uint32_t m_RendererId = 0;
        FramebufferSpecification m_Specification;

        std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
        FramebufferTextureSpecification m_DepthAttachmentSpecification;

        std::vector<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment = 0;
    };
}
