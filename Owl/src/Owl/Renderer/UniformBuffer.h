#pragma once

namespace Owl
{
    class UniformBuffer
    {
    public:
        virtual ~UniformBuffer() {}
        virtual void SetData(const void* pData, uint32_t pSize, uint32_t pOffset = 0) = 0;
		
        static Ref<UniformBuffer> Create(uint32_t pSize, uint32_t pBinding);
    };
}
