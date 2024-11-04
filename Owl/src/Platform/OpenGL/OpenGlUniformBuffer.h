#pragma once
#include "Owl/Renderer/UniformBuffer.h"

namespace Owl
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t pSize, uint32_t pBinding);
		~OpenGLUniformBuffer() override;
		void SetData(const void* pData, uint32_t pSize, uint32_t pOffset = 0) override;

	private:
		uint32_t m_RendererID = 0;
	};
}
