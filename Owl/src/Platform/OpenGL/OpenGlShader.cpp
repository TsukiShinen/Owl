#include "opch.h"
#include "OpenGlShader.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace Owl
{
	static GLenum ShaderTypeFromString(const std::string& pType)
	{
		if (pType == "vertex")
			return GL_VERTEX_SHADER;
		if (pType == "fragment" || pType == "pixel")
			return GL_FRAGMENT_SHADER;

		OWL_CORE_ASSERT(false, "Unknown shader type!")
		return 0;
	}

	OpenGlShader::OpenGlShader(const std::string& pFilePath)
	{
		const std::string source = ReadFile(pFilePath);
		const auto shaderSources = PreProcess(source);
		Compile(shaderSources);
	}

	OpenGlShader::OpenGlShader(const std::string& pVertexSource, const std::string& pFragmentSource)
	{
		std::unordered_map<GLenum, std::string> sources{
			{ GL_VERTEX_SHADER, pVertexSource },
			{ GL_FRAGMENT_SHADER, pFragmentSource },
		};
		Compile(sources);
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

	void OpenGlShader::UploadUniformInt(const std::string& pName, const int pValue) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, pName.c_str());
		glUniform1i(location, pValue);
	}

	void OpenGlShader::UploadUniformFloat(const std::string& pName, const float pValue) const
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

	std::string OpenGlShader::ReadFile(const std::string& pFilePath)
	{
		std::string result;
		std::ifstream in(pFilePath, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(result.data(), result.size());
			in.close();
		}
		else
			OWL_CORE_ERROR("Could not open file '{0}'", pFilePath);

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGlShader::PreProcess(const std::string& pSource)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const auto typeToken = "#type";
		const size_t typeTokenLength = strlen(typeToken);
		size_t pos = pSource.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			const size_t eol = pSource.find_first_of("\r\n", pos);
			OWL_CORE_ASSERT(eol != std::string::npos, "Syntax error")
			const size_t begin = pos + typeTokenLength + 1;
			std::string type = pSource.substr(begin, eol - begin);
			OWL_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified")

			const size_t nextLinePos = pSource.find_first_not_of("\r\n", eol);
			OWL_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error")
			pos = pSource.find(typeToken, nextLinePos);

			shaderSources[ShaderTypeFromString(type)] = pos == std::string::npos
				                                            ? pSource.substr(nextLinePos)
				                                            : pSource.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGlShader::Compile(const std::unordered_map<GLenum, std::string>& pShaderSources)
	{
		const GLint program = glCreateProgram();
		std::vector<GLenum> glShaderIds(pShaderSources.size());

		for (auto& kv : pShaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			const GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, nullptr);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());

				glDeleteShader(shader);

				OWL_CORE_ERROR("{0}", infoLog.data());
				OWL_ASSERT(false, "Shader compilation failure!")

				break;
			}

			glAttachShader(program, shader);
			glShaderIds.push_back(shader);
		}


		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());

			glDeleteProgram(program);
			for (const auto id : glShaderIds)
				glDeleteShader(id);

			OWL_CORE_ERROR("{0}", infoLog.data());
			OWL_ASSERT(false, "Shader Link failure!")

			return;
		}

		for (const auto id : glShaderIds)
			glDetachShader(program, id);

		m_RendererID = program;
	}
}
