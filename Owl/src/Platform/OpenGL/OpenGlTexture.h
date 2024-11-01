#pragma once

#include <glad/glad.h>

#include "Owl/Renderer/Texture.h"

namespace Owl
{
	class OpenGlTexture2D : public Texture2D
	{
	public:
		OpenGlTexture2D(uint32_t pWidth, uint32_t pHeight);
		OpenGlTexture2D(std::string pPath);
		~OpenGlTexture2D() override;

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		uint32_t GetRendererID() const override { return m_RendererId; }

		void SetData(void* pData, uint32_t pSize) override;

		void Bind(uint32_t pSlot = 0) const override;

		bool IsLoaded() const override { return m_IsLoaded; }
		
		bool operator==(const Texture& other) const override
		{
			return m_RendererId == ((OpenGlTexture2D&)other).m_RendererId;
		}

	private:
		std::string m_Path;
		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererId;
		GLenum m_InternalFormat, m_DataFormat;
	};
}
