#include "opch.h"
#include "OpenGlTexture.h"

#include "stb_image.h"
#include "glad/glad.h"

namespace Owl
{
	OpenGlTexture2D::OpenGlTexture2D(const uint32_t pWidth, const uint32_t pHeight)
		: m_Width(pWidth), m_Height(pHeight)
	{
		OWL_PROFILE_FUNCTION();
		
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		glTextureStorage2D(m_RendererId, 1, m_InternalFormat, static_cast<GLsizei>(m_Width),
						   static_cast<GLsizei>(m_Height));

		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGlTexture2D::OpenGlTexture2D(std::string pPath)
		: m_Path(std::move(pPath))
	{
		OWL_PROFILE_FUNCTION();
		
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);
		OWL_CORE_ASSERT(data, "Failed to load image!")
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0;
		GLenum dataFormat = 0;
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

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		glTextureStorage2D(m_RendererId, 1, m_InternalFormat, static_cast<GLsizei>(m_Width),
		                   static_cast<GLsizei>(m_Height));

		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererId, 0, 0, 0, static_cast<GLsizei>(m_Width), static_cast<GLsizei>(m_Height),
		                    m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGlTexture2D::~OpenGlTexture2D()
	{
		OWL_PROFILE_FUNCTION();
		
		glDeleteTextures(1, &m_RendererId);
	}

	void OpenGlTexture2D::SetData(void* pData, const uint32_t pSize)
	{
		OWL_PROFILE_FUNCTION();
		
		OWL_CORE_ASSERT(pSize == m_Width * m_Height * (m_DataFormat == GL_RGBA ? 4 : 3), "Data must be entire texture!")
		glTextureSubImage2D(m_RendererId, 0, 0, 0, static_cast<GLsizei>(m_Width), static_cast<GLsizei>(m_Height),
							m_DataFormat, GL_UNSIGNED_BYTE, pData);
	}

	void OpenGlTexture2D::Bind(const uint32_t pSlot) const
	{
		OWL_PROFILE_FUNCTION();
		
		glBindTextureUnit(pSlot, m_RendererId);
	}
}
