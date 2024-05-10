#include "opch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGlShader.h"

namespace Owl
{
	Ref<Shader> Shader::Create(const std::string& pFilePath)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			OWL_CORE_ASSERT(false, "RandererApi::None is currently not supported!")
			return nullptr;
		case RendererApi::Api::OpenGl:
			return CreateRef<OpenGlShader>(pFilePath);
		}

		OWL_CORE_ASSERT(false, "Unknown RendererApi!")
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& pName, const std::string& pVertexSource,
	                           const std::string& pFragmentSource)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			OWL_CORE_ASSERT(false, "RandererApi::None is currently not supported!")
			return nullptr;
		case RendererApi::Api::OpenGl:
			return CreateRef<OpenGlShader>(pName, pVertexSource, pFragmentSource);
		}

		OWL_CORE_ASSERT(false, "Unknown RendererApi!")
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& pShader)
	{
		Add(pShader->GetName(), pShader);
	}

	void ShaderLibrary::Add(const std::string& pName, const Ref<Shader>& pShader)
	{
		OWL_CORE_ASSERT(!Exists(pName), "Shader already exist!")
		m_Shaders[pName] = pShader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& pFilePath)
	{
		const auto shader = Shader::Create(pFilePath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& pName, const std::string& pFilePath)
	{
		const auto shader = Shader::Create(pFilePath);
		Add(pName, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& pName)
	{
		OWL_CORE_ASSERT(Exists(pName), "Shader not found!")
		return m_Shaders[pName];
	}

	bool ShaderLibrary::Exists(const std::string& pName) const
	{
		return m_Shaders.contains(pName);
	}
}
