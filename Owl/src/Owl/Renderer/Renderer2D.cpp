#include "opch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Owl
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;

		Ref<Texture2D> WhiteTexture;
		
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_Storage;

	void Renderer2D::Init()
	{
		OWL_PROFILE_FUNCTION();
		
		s_Storage = new Renderer2DStorage();
		s_Storage->QuadVertexArray = VertexArray::Create();

		float quadVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};
		Ref<VertexBuffer> quadVertexBuffer = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		quadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "in_Position"},
			{ShaderDataType::Float2, "in_TexCoord"}
		});
		s_Storage->QuadVertexArray->AddVertexBuffer(quadVertexBuffer);

		uint32_t quadIndices[6] = {0, 1, 2, 2, 3, 0};
		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t));
		s_Storage->QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

		s_Storage->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Storage->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		
		s_Storage->TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		s_Storage->TextureShader->Bind();
		s_Storage->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		OWL_PROFILE_FUNCTION();
		
		delete s_Storage;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& pCamera)
	{
		OWL_PROFILE_FUNCTION();
		
		s_Storage->TextureShader->Bind();
		s_Storage->TextureShader->SetMat4("u_ViewProjection", pCamera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		OWL_PROFILE_FUNCTION();
		
	}

	void Renderer2D::DrawQuad(const glm::vec2& pPosition, const glm::vec2& pSize, const glm::vec4& pColor)
	{
		DrawQuad({pPosition.x, pPosition.y, 0.0f}, pSize, s_Storage->WhiteTexture, 1.0f, pColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pPosition, const glm::vec2& pSize, const glm::vec4& pColor)
	{
		DrawQuad(pPosition, pSize, s_Storage->WhiteTexture, 1.0f, pColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pPosition, const glm::vec2& pSize, const Ref<Texture2D>& pTexture, const float pTilingFactor, const glm::vec4& pColor)
	{
		DrawQuad({pPosition.x, pPosition.y, 0.0f}, pSize, pTexture, pTilingFactor, pColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pPosition, const glm::vec2& pSize, const Ref<Texture2D>& pTexture, const float pTilingFactor, const glm::vec4& pColor)
	{
		OWL_PROFILE_FUNCTION();
		
		s_Storage->TextureShader->SetFloat4("u_Color", pColor);
		s_Storage->TextureShader->SetFloat("u_TilingFactor", pTilingFactor);
		pTexture->Bind();

		glm::mat4 transform = translate(glm::mat4(1.0f), pPosition)
							* scale(glm::mat4(1.0f), {pSize.x, pSize.y, 1.0f});
		s_Storage->TextureShader->SetMat4("u_Transform", transform);

		s_Storage->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Storage->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pPosition, float pRotation, const glm::vec2& pSize, const glm::vec4& pColor)
	{
		DrawRotatedQuad({pPosition.x, pPosition.y, 0}, pRotation, pSize, s_Storage->WhiteTexture, 1.0f, pColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pPosition, float pRotation, const glm::vec2& pSize, const glm::vec4& pColor)
	{
		DrawRotatedQuad(pPosition, pRotation, pSize, s_Storage->WhiteTexture, 1.0f, pColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pPosition, float pRotation, const glm::vec2& pSize, const Ref<Texture2D>& pTexture, const float pTilingFactor, const glm::vec4& pColor)
	{
		DrawRotatedQuad({pPosition.x, pPosition.y, 0}, pRotation, pSize, pTexture, pTilingFactor, pColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pPosition, float pRotation, const glm::vec2& pSize, const Ref<Texture2D>& pTexture, float pTilingFactor, const glm::vec4& pColor)
	{
		OWL_PROFILE_FUNCTION();
		
		s_Storage->TextureShader->SetFloat4("u_Color", pColor);
		s_Storage->TextureShader->SetFloat("u_TilingFactor", pTilingFactor);
		pTexture->Bind();

		glm::mat4 transform = translate(glm::mat4(1.0f), pPosition)
							* rotate(glm::mat4(1.0f), pRotation, { 0.0f, 0.0f, 1.0f })
							* scale(glm::mat4(1.0f), {pSize.x, pSize.y, 1.0f});
		s_Storage->TextureShader->SetMat4("u_Transform", transform);

		s_Storage->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Storage->QuadVertexArray);
	}
}
