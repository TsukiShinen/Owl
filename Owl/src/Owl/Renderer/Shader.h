#pragma once

namespace Owl
{
	class Shader
	{
	public:
		Shader(const std::string& pVertexSource, const std::string& pFragmentSource);
		~Shader();

		void Bind() const;
		void UnBind() const;
	private:
		uint32_t m_RendererID;
	};
	
}
