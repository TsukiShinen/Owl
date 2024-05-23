﻿#include "opch.h"
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
		OWL_PROFILE_FUNCTION();
		
		const std::string source = ReadFile(pFilePath);
		const auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract Name from file path
		auto lastSlash = pFilePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = pFilePath.rfind('.');
		auto count = lastDot == std::string::npos ? pFilePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = pFilePath.substr(lastSlash, count);
	}

	OpenGlShader::OpenGlShader(std::string pName, const std::string& pVertexSource,
	                           const std::string& pFragmentSource)
		: m_Name(std::move(pName))
	{
		OWL_PROFILE_FUNCTION();
		
		Compile({
			{GL_VERTEX_SHADER, pVertexSource},
			{GL_FRAGMENT_SHADER, pFragmentSource},
		});
	}

	OpenGlShader::~OpenGlShader()
	{
		OWL_PROFILE_FUNCTION();
		
		glDeleteProgram(m_RendererID);
	}

	void OpenGlShader::Bind() const
	{
		OWL_PROFILE_FUNCTION();
		
		glUseProgram(m_RendererID);
	}

	void OpenGlShader::UnBind() const
	{
		OWL_PROFILE_FUNCTION();
		
		glUseProgram(0);
	}

	void OpenGlShader::SetMat4(const std::string& pName, const glm::mat4& pValue)
	{
		OWL_PROFILE_FUNCTION();
		
		UploadUniformMat4(pName, pValue);
	}

	void OpenGlShader::SetFloat4(const std::string& pName, const glm::vec4& pValue)
	{
		OWL_PROFILE_FUNCTION();
		
		UploadUniformFloat4(pName, pValue);
	}

	void OpenGlShader::SetFloat3(const std::string& pName, const glm::vec3& pValue)
	{
		OWL_PROFILE_FUNCTION();
		
		UploadUniformFloat3(pName, pValue);
	}

	void OpenGlShader::SetInt(const std::string& pName, const int pValue)
	{
		OWL_PROFILE_FUNCTION();
		
		UploadUniformInt(pName, pValue);
	}

	void OpenGlShader::SetIntArray(const std::string& pName, std::span<int> pValues)
	{
		OWL_PROFILE_FUNCTION();

		UploadUniformIntArray(pName, pValues);
	}

	void OpenGlShader::SetFloat(const std::string& pName, float pValue)
	{
		OWL_PROFILE_FUNCTION();
		
		UploadUniformFloat(pName, pValue);
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

	void OpenGlShader::UploadUniformIntArray(const std::string& pName, const std::span<int> pValues) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, pName.c_str());
		glUniform1iv(location, pValues.size(), pValues.data());
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
		OWL_PROFILE_FUNCTION();
		
		std::string result;
		std::ifstream in(pFilePath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(result.data(), static_cast<std::streamsize>(result.size()));
			in.close();
		}
		else
			OWL_CORE_ERROR("Could not open file '{0}'", pFilePath);

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGlShader::PreProcess(const std::string& pSource)
	{
		OWL_PROFILE_FUNCTION();
		
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
		OWL_PROFILE_FUNCTION();
		
		const GLint program = glCreateProgram();
		std::vector<GLenum> glShaderIds;
		glShaderIds.reserve(pShaderSources.size());

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
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}
}
