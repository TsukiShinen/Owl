#pragma once
#include "Owl/Renderer/Texture.h"

namespace Owl
{
	class OpenGlTexture2D : public Texture2D
	{
	public:
		OpenGlTexture2D(std::string pPath);
		~OpenGlTexture2D() override;

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		void Bind(uint32_t pSlot = 0) const override;

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererId;
	};
}
