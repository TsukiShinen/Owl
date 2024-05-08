#include "opch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGlShader.h"

namespace Owl
{
	Shader* Shader::Create(const std::string& pFilePath)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			OWL_CORE_ASSERT(false, "RandererApi::None is currently not supported!")
			return nullptr;
		case RendererApi::Api::OpenGl:
			return new OpenGlShader(pFilePath);
		}

		OWL_CORE_ASSERT(false, "Unknown RendererApi!")
		return nullptr;
	}

	Shader* Shader::Create(const std::string& pVertexSource, const std::string& pFragmentSource)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			OWL_CORE_ASSERT(false, "RandererApi::None is currently not supported!")
			return nullptr;
		case RendererApi::Api::OpenGl:
			return new OpenGlShader(pVertexSource, pFragmentSource);
		}

		OWL_CORE_ASSERT(false, "Unknown RendererApi!")
		return nullptr;
	}
}
