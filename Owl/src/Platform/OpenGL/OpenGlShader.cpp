#include "opch.h"
#include "OpenGlShader.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "Owl/Core/Timer.h"

namespace Owl
{
	namespace Utils
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
		
		static shaderc_shader_kind GLShaderStageToShaderC(const GLenum pStage)
		{
			switch (pStage)
			{
			case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
			}
			OWL_CORE_ASSERT(false)
			return static_cast<shaderc_shader_kind>(0);
		}
		
		static const char* GLShaderStageToString(const GLenum pStage)
		{
			switch (pStage)
			{
			case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
			}
			OWL_CORE_ASSERT(false)
			return nullptr;
		}
		
		static const char* GetCacheDirectory()
		{
			// TODO: make sure the assets directory is valid
			return "assets/cache/shader/opengl";
		}
		
		static void CreateCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}
		
		static const char* GLShaderStageCachedOpenGLFileExtension(const uint32_t pStage)
		{
			switch (pStage)
			{
			case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
			case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
			}
			OWL_CORE_ASSERT(false);
			return "";
		}
		
		static const char* GLShaderStageCachedVulkanFileExtension(const uint32_t pStage)
		{
			switch (pStage)
			{
			case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
			case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
			}
			OWL_CORE_ASSERT(false)
			return "";
		}
	}

	OpenGlShader::OpenGlShader(const std::string& pFilePath)
		: m_FilePath(pFilePath)
	{
		OWL_PROFILE_FUNCTION();
		
		Utils::CreateCacheDirectoryIfNeeded();
		
		const std::string source = ReadFile(pFilePath);
		const auto shaderSources = PreProcess(source);
		{
			Timer timer;
			CompileOrGetVulkanBinaries(shaderSources);
			CompileOrGetOpenGlBinaries();
			CreateProgram();
			OWL_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
		}

		// Extract Name from file path
		auto lastSlash = pFilePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		const auto lastDot = pFilePath.rfind('.');
		const auto count = lastDot == std::string::npos ? pFilePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = pFilePath.substr(lastSlash, count);
	}

	OpenGlShader::OpenGlShader(std::string pName, const std::string& pVertexSource,
	                           const std::string& pFragmentSource)
		: m_Name(std::move(pName))
	{
		OWL_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = pVertexSource;
		sources[GL_FRAGMENT_SHADER] = pFragmentSource;
		
		CompileOrGetVulkanBinaries(sources);
		CompileOrGetOpenGlBinaries();
		CreateProgram();
	}

	OpenGlShader::~OpenGlShader()
	{
		OWL_PROFILE_FUNCTION();
		
		glDeleteProgram(m_RendererId);
	}

	void OpenGlShader::Bind() const
	{
		OWL_PROFILE_FUNCTION();
		
		glUseProgram(m_RendererId);
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

	void OpenGlShader::SetFloat2(const std::string& pName, const glm::vec2& pValue)
	{
		OWL_PROFILE_FUNCTION();
		
		UploadUniformFloat2(pName, pValue);
	}

	void OpenGlShader::UploadUniformInt(const std::string& pName, const int pValue) const
	{
		const GLint location = glGetUniformLocation(m_RendererId, pName.c_str());
		glUniform1i(location, pValue);
	}

	void OpenGlShader::UploadUniformFloat(const std::string& pName, const float pValue) const
	{
		const GLint location = glGetUniformLocation(m_RendererId, pName.c_str());
		glUniform1f(location, pValue);
	}

	void OpenGlShader::UploadUniformIntArray(const std::string& pName, const std::span<int> pValues) const
	{
		const GLint location = glGetUniformLocation(m_RendererId, pName.c_str());
		glUniform1iv(location, pValues.size(), pValues.data());
	}

	void OpenGlShader::UploadUniformFloat2(const std::string& pName, const glm::vec2& pValues) const
	{
		const GLint location = glGetUniformLocation(m_RendererId, pName.c_str());
		glUniform2f(location, pValues.x, pValues.y);
	}

	void OpenGlShader::UploadUniformFloat3(const std::string& pName, const glm::vec3& pValues) const
	{
		const GLint location = glGetUniformLocation(m_RendererId, pName.c_str());
		glUniform3f(location, pValues.x, pValues.y, pValues.z);
	}

	void OpenGlShader::UploadUniformFloat4(const std::string& pName, const glm::vec4& pValues) const
	{
		const GLint location = glGetUniformLocation(m_RendererId, pName.c_str());
		glUniform4f(location, pValues.x, pValues.y, pValues.z, pValues.w);
	}

	void OpenGlShader::UploadUniformMat3(const std::string& pName, const glm::mat3& pMatrix) const
	{
		const GLint location = glGetUniformLocation(m_RendererId, pName.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(pMatrix));
	}

	void OpenGlShader::UploadUniformMat4(const std::string& pName, const glm::mat4& pMatrix) const
	{
		const GLint location = glGetUniformLocation(m_RendererId, pName.c_str());
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
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(result.data(), size);
				in.close();
			}
			else
			{
				OWL_CORE_ERROR("Could not read from file '{0}'", pFilePath);
			}
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
			OWL_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified")

			const size_t nextLinePos = pSource.find_first_not_of("\r\n", eol);
			OWL_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error")
			pos = pSource.find(typeToken, nextLinePos);

			shaderSources[Utils::ShaderTypeFromString(type)] = pos == std::string::npos
				                                            ? pSource.substr(nextLinePos)
				                                            : pSource.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGlShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& pShaderSources)
	{
		GLuint program = glCreateProgram();

		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		if (constexpr bool optimize = true)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto& shaderData = m_VulkanSpirv;
		shaderData.clear();
		for (auto&& [stage, source] : pShaderSources)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::Compiler compiler;
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					OWL_CORE_ERROR(module.GetErrorMessage());
					OWL_CORE_ASSERT(false)
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				if (std::ofstream out(cachedPath, std::ios::out | std::ios::binary); out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data]: shaderData)
			Reflect(stage, data);
	}

	void OpenGlShader::CompileOrGetOpenGlBinaries()
	{
		auto& shaderData = m_OpenGlSpirv;

		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		if (constexpr bool optimize = false)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		shaderData.clear();
		m_OpenGLSourceCode.clear();
		for (auto&& [stage, spirv] : m_VulkanSpirv)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::Compiler compiler;
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_OpenGLSourceCode[stage] = glslCompiler.compile();
				auto& source = m_OpenGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					OWL_CORE_ERROR(module.GetErrorMessage());
					OWL_CORE_ASSERT(false)
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				if (std::ofstream out(cachedPath, std::ios::out | std::ios::binary); out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGlShader::CreateProgram()
	{
		const GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_OpenGlSpirv)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			OWL_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());

			glDeleteProgram(program);

			for (const auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (const auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererId = program;
	}

	void OpenGlShader::Reflect(GLenum pStage, const std::vector<uint32_t>& pShaderData)
	{
		spirv_cross::Compiler compiler(pShaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		OWL_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(pStage), m_FilePath);
		OWL_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		OWL_CORE_TRACE("    {0} resources", resources.sampled_images.size());

		OWL_CORE_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			OWL_CORE_TRACE("  {0}", resource.name);
			OWL_CORE_TRACE("    Size = {0}", bufferSize);
			OWL_CORE_TRACE("    Binding = {0}", binding);
			OWL_CORE_TRACE("    Members = {0}", memberCount);
		}
	}
}
