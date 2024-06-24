#pragma once

#include "Owl/Core/Base.h"

namespace Owl
{
    struct FramebufferSpecification
    {
        uint32_t Width, Height;
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
        
        virtual uint32_t GetColorAttachmentRendererId() const = 0;
        virtual const FramebufferSpecification& GetSpecification() const = 0;
        
        static Ref<Framebuffer> Create(const FramebufferSpecification& pSpecification);
    };
}