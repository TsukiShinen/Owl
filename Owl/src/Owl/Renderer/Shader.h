#pragma once
#include "glm/glm.hpp"

namespace Owl
{
	class Shader
	{
	public:
		Shader(const std::string& pVertexSource, const std::string& pFragmentSource);
		~Shader();

		void Bind() const;
		void UnBind() const;

		void UploadUniformMat4(const std::string& pName, const glm::mat4& pMatrix);
	private:
		uint32_t m_RendererID;
	};
	
}
