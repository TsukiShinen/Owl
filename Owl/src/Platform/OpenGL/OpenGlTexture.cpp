#include "opch.h"
#include "OpenGlTexture.h"

#include "stb_image.h"
#include "glad/glad.h"

namespace Owl
{
	OpenGlTexture2D::OpenGlTexture2D(std::string pPath)
		: m_Path(std::move(pPath))
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);
		OWL_CORE_ASSERT(data, "Failed to load image!")
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		OWL_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!")

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		glTextureStorage2D(m_RendererId, 1, internalFormat, static_cast<GLsizei>(m_Width),
		                   static_cast<GLsizei>(m_Height));

		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererId, 0, 0, 0, static_cast<GLsizei>(m_Width), static_cast<GLsizei>(m_Height),
		                    dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGlTexture2D::~OpenGlTexture2D()
	{
		glDeleteTextures(1, &m_RendererId);
	}

	void OpenGlTexture2D::Bind(const uint32_t pSlot) const
	{
		glBindTextureUnit(pSlot, m_RendererId);
	}
}
