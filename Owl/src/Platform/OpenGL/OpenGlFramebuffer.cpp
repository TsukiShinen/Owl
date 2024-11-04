#include "opch.h"
#include "OpenGlFramebuffer.h"

#include <glad/glad.h>

namespace Owl
{
	static uint32_t s_MaxFrameBufferSize = 8192;

	namespace Utils
	{
		static GLenum TextureTarget(const bool pMultisample)
		{
			return pMultisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTexture(const bool pMultisample, uint32_t* pOutId, const uint32_t pCount)
		{
			glCreateTextures(TextureTarget(pMultisample), pCount, pOutId);
		}

		static void BindTexture(bool pMultisample, uint32_t pId)
		{
			glBindTexture(TextureTarget(pMultisample), pId);
		}

		static void AttachColorTexture(const uint32_t pId, const int pSamples, const GLenum pInternalFormat,
		                               const GLenum pFormat,
		                               const uint32_t pWidth, const uint32_t pHeight, const int pIndex)
		{
			const bool multisample = pSamples > 1;
			if (multisample)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, pSamples, pInternalFormat, pWidth, pHeight,
				                        GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, pInternalFormat, pWidth, pHeight, 0, pFormat, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + pIndex, TextureTarget(multisample), pId, 0);
		}

		static void AttachDepthTexture(const uint32_t pId, const uint32_t pSamples, const GLenum pFormat,
		                               GLenum pAttachmentType, const uint32_t pWidth, const uint32_t pHeight)
		{
			const bool multisample = pSamples > 1;
			if (multisample)
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, pSamples, pFormat, pWidth, pHeight, GL_FALSE);
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, pFormat, pWidth, pHeight);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, pAttachmentType, TextureTarget(multisample), pId, 0);
		}

		static bool IsDepthFormat(FramebufferTextureFormat pFormat)
		{
			switch (pFormat)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
			}

			return false;
		}

		static GLenum OwlFBTextureFormatToGl(const FramebufferTextureFormat pFormat)
		{
			switch (pFormat)
			{
				case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
				case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}
			OWL_CORE_ASSERT(false)
			return 0;
		}
	}

	OpenGlFramebuffer::OpenGlFramebuffer(const FramebufferSpecification& pSpecification)
		: m_Specification(pSpecification)
	{
		for (auto format : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(format.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(format);
			else
				m_DepthAttachmentSpecification = format;
		}

		Invalidate();
	}

	OpenGlFramebuffer::~OpenGlFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererId);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGlFramebuffer::Resize(uint32_t pWidth, uint32_t pHeight)
	{
		if (pWidth == 0 || pHeight == 0 || pWidth > s_MaxFrameBufferSize || pHeight > s_MaxFrameBufferSize)
		{
			OWL_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", pWidth, pHeight);
			return;
		}

		m_Specification.Width = pWidth;
		m_Specification.Height = pHeight;

		Invalidate();
	}

	int OpenGlFramebuffer::ReadPixel(uint32_t pAttachmentIndex, int pX, int pY)
	{
		OWL_CORE_ASSERT(pAttachmentIndex < m_ColorAttachments.size())

		glReadBuffer(GL_COLOR_ATTACHMENT0 + pAttachmentIndex);
		int pixelData;
		glReadPixels(pX, pY, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGlFramebuffer::ClearAttachment(uint32_t pAttachmentIndex, int pValue)
	{
		OWL_CORE_ASSERT(pAttachmentIndex < m_ColorAttachments.size())

		glClearTexImage(m_ColorAttachments[pAttachmentIndex], 0,
		                Utils::OwlFBTextureFormatToGl(m_ColorAttachmentSpecifications[pAttachmentIndex].TextureFormat),
		                GL_INT, &pValue);
	}

	void OpenGlFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGlFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGlFramebuffer::Invalidate()
	{
		if (m_RendererId)
		{
			glDeleteFramebuffers(1, &m_RendererId);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

		const bool multisample = m_Specification.Samples > 1;

		// Attachments
		if (!m_ColorAttachmentSpecifications.empty())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			Utils::CreateTexture(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (uint32_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
					case FramebufferTextureFormat::RGBA8:
						Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA,
						                          m_Specification.Width, m_Specification.Height, i);
						break;
					case FramebufferTextureFormat::RED_INTEGER:
						Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I,
						                          GL_RED_INTEGER,
						                          m_Specification.Width, m_Specification.Height, i);
						break;
					default:
						break;
				}
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTexture(multisample, &m_DepthAttachment, 1);
			Utils::BindTexture(multisample, m_DepthAttachment);
			switch (m_DepthAttachmentSpecification.TextureFormat)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8,
					                          GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width,
					                          m_Specification.Height);
					break;
				default:
					break;
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			OWL_CORE_ASSERT(m_ColorAttachments.size() <= 4);
			GLenum buffers[4] = {
				GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3
			};
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		OWL_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
		                "Framebuffer is incomplete!")

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
