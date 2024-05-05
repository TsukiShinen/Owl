#pragma once
#include "glm/glm.hpp"
#include "Owl/Renderer/Shader.h"

namespace Owl
{
	class OpenGlShader : public Shader
	{
	public:
		OpenGlShader(const std::string& pVertexSource, const std::string& pFragmentSource);
		~OpenGlShader() override;

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
		uint32_t m_RendererID;
	};
	
}
