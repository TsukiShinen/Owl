#pragma once

#include "glm/glm.hpp"
#include "Owl/Renderer/Shader.h"

//TODO : Remove
using GLenum = unsigned int;

namespace Owl
{
	class OpenGlShader : public Shader
	{
	public:
		OpenGlShader(const std::string& pFilePath);
		OpenGlShader(const std::string& pName, const std::string& pVertexSource, const std::string& pFragmentSource);
		~OpenGlShader() override;
		
		const std::string& GetName() const override { return m_Name; }

		void Bind() const override;
		void UnBind() const override;
		
		void UploadUniformInt(const std::string& pName, int pValue) const;

		void UploadUniformFloat(const std::string& pName, float pValue) const;
		void UploadUniformFloat2(const std::string& pName, const glm::vec2& pValues) const;
		void UploadUniformFloat3(const std::string& pName, const glm::vec3& pValues) const;
		void UploadUniformFloat4(const std::string& pName, const glm::vec4& pValues) const;

		void UploadUniformMat3(const std::string& pName, const glm::mat3& pMatrix) const;
		void UploadUniformMat4(const std::string& pName, const glm::mat4& pMatrix) const;

	private:
		static std::string ReadFile(const std::string& pFilePath);
		static std::unordered_map<GLenum, std::string> PreProcess(const std::string& pSource);
		void Compile(const std::unordered_map<GLenum, std::string>& pShaderSources);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
	
}
