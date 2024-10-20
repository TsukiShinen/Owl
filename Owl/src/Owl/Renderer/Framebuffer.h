#pragma once

#include "Owl/Core/Base.h"

namespace Owl
{
    enum class FramebufferTextureFormat
    {
        None = 0,

        // Color
        RGBA8,
        RED_INTEGER,

        // Depth/Stencil
        DEPTH24STENCIL8,

        // Defaults
        Depth = DEPTH24STENCIL8
    };

    struct FramebufferTextureSpecification
    {
        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(FramebufferTextureFormat pFormat)
            : TextureFormat(pFormat) { }

        FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
        // TODO: Filtering/wrap
    };

    struct FramebufferAttachmentSpecification
    {
        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> pAttachments)
            : Attachments(pAttachments) { }

        std::vector<FramebufferTextureSpecification> Attachments;
    };
    
    struct FramebufferSpecification
    {
        uint32_t Width, Height;
        FramebufferAttachmentSpecification Attachments;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };
    
    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;
        
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        
		virtual void Resize(uint32_t pWidth, uint32_t pHeight) = 0;
        virtual int ReadPixel(uint32_t pAttachmentPixel, int pX, int pY) = 0;
        
        virtual uint32_t GetColorAttachmentRendererId(uint32_t pIndex = 0) const = 0;
        virtual const FramebufferSpecification& GetSpecification() const = 0;
        
        static Ref<Framebuffer> Create(const FramebufferSpecification& pSpecification);
    };
}