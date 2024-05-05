#include "opch.h"
#include "OpenGlShader.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace Owl
{
	OpenGlShader::OpenGlShader(const std::string& pVertexSource, const std::string& pFragmentSource)
	{
		// ===== VERTEX SHADER =====
		const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = pVertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, nullptr);

		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog.data());

			glDeleteShader(vertexShader);

			OWL_CORE_ERROR("{0}", infoLog.data());
			OWL_ASSERT(false, "Vertex Shader compilation failure!")

			return;
		}

		// ===== FRAGMENT SHADER =====
		const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = pFragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, nullptr);

		glCompileShader(fragmentShader);
		
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog.data());

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			OWL_CORE_ERROR("{0}", infoLog.data());
			OWL_ASSERT(false, "Fragment Shader compilation failure!")

			return;
		}

		// ===== LINK =====
		m_RendererID = glCreateProgram();

		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		glLinkProgram(m_RendererID);

		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, infoLog.data());

			glDeleteProgram(m_RendererID);
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			OWL_CORE_ERROR("{0}", infoLog.data());
			OWL_ASSERT(false, "Shader Link failure!")

			return;
		}

		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
	}

	OpenGlShader::~OpenGlShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGlShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGlShader::UnBind() const
	{
		glUseProgram(0);
	}

	void OpenGlShader::UploadUniformInt(const std::string& pName, int pValue) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, pName.c_str());
		glUniform1i(location, pValue);
	}

	void OpenGlShader::UploadUniformFloat(const std::string& pName, float pValue) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, pName.c_str());
		glUniform1f(location, pValue);
	}

	void OpenGlShader::UploadUniformFloat2(const std::string& pName, const glm::vec2& pValues) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, pName.c_str());
		glUniform2f(location, pValues.x, pValues.y);
	}

	void OpenGlShader::UploadUniformFloat3(const std::string& pName, const glm::vec3& pValues) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, pName.c_str());
		glUniform3f(location, pValues.x, pValues.y, pValues.z);
	}

	void OpenGlShader::UploadUniformFloat4(const std::string& pName, const glm::vec4& pValues) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, pName.c_str());
		glUniform4f(location, pValues.x, pValues.y, pValues.z, pValues.w);
	}

	void OpenGlShader::UploadUniformMat3(const std::string& pName, const glm::mat3& pMatrix) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, pName.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(pMatrix));
	}
	
	void OpenGlShader::UploadUniformMat4(const std::string& pName, const glm::mat4& pMatrix) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, pName.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(pMatrix));
	}
}
