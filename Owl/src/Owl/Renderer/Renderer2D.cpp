#include "opch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Owl
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};
	
	struct Renderer2DData
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Texture2D> WhiteTexture;
		Ref<Shader> TextureShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		OWL_PROFILE_FUNCTION();
		
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "in_Position"},
			{ShaderDataType::Float4, "in_Color"},
			{ShaderDataType::Float2, "in_TexCoord"}
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			
			offset += 4;
		}
		
		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIndexBuffer);
		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		
		s_Data.TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		OWL_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& pCamera)
	{
		OWL_PROFILE_FUNCTION();
		
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", pCamera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		OWL_PROFILE_FUNCTION();

		uint32_t dataSize = reinterpret_cast<uint8_t*>(s_Data.QuadVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pPosition, const glm::vec2& pSize, const glm::vec4& pColor)
	{
		DrawQuad({pPosition.x, pPosition.y, 0.0f}, pSize, s_Data.WhiteTexture, 1.0f, pColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pPosition, const glm::vec2& pSize, const glm::vec4& pColor)
	{
		DrawQuad(pPosition, pSize, s_Data.WhiteTexture, 1.0f, pColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pPosition, const glm::vec2& pSize, const Ref<Texture2D>& pTexture, const float pTilingFactor, const glm::vec4& pColor)
	{
		DrawQuad({pPosition.x, pPosition.y, 0.0f}, pSize, pTexture, pTilingFactor, pColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pPosition, const glm::vec2& pSize, const Ref<Texture2D>& pTexture, const float pTilingFactor, const glm::vec4& pColor)
	{
		OWL_PROFILE_FUNCTION();

		s_Data.QuadVertexBufferPtr->Position = pPosition;
		s_Data.QuadVertexBufferPtr->Color = pColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { pPosition.x + pSize.x, pPosition.y, 0.0f};
		s_Data.QuadVertexBufferPtr->Color = pColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { pPosition.x + pSize.x, pPosition.y + pSize.y, 0.0f};
		s_Data.QuadVertexBufferPtr->Color = pColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { pPosition.x, pPosition.y + pSize.y, 0.0f};
		s_Data.QuadVertexBufferPtr->Color = pColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
		/*
		s_Data.TextureShader->SetFloat4("u_Color", pColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", pTilingFactor);
		pTexture->Bind();

		glm::mat4 transform = translate(glm::mat4(1.0f), pPosition)
							* scale(glm::mat4(1.0f), {pSize.x, pSize.y, 1.0f});
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);*/
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pPosition, float pRotation, const glm::vec2& pSize, const glm::vec4& pColor)
	{
		DrawRotatedQuad({pPosition.x, pPosition.y, 0}, pRotation, pSize, s_Data.WhiteTexture, 1.0f, pColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pPosition, float pRotation, const glm::vec2& pSize, const glm::vec4& pColor)
	{
		DrawRotatedQuad(pPosition, pRotation, pSize, s_Data.WhiteTexture, 1.0f, pColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pPosition, float pRotation, const glm::vec2& pSize, const Ref<Texture2D>& pTexture, const float pTilingFactor, const glm::vec4& pColor)
	{
		DrawRotatedQuad({pPosition.x, pPosition.y, 0}, pRotation, pSize, pTexture, pTilingFactor, pColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pPosition, float pRotation, const glm::vec2& pSize, const Ref<Texture2D>& pTexture, float pTilingFactor, const glm::vec4& pColor)
	{
		OWL_PROFILE_FUNCTION();
		
		s_Data.TextureShader->SetFloat4("u_Color", pColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", pTilingFactor);
		pTexture->Bind();

		glm::mat4 transform = translate(glm::mat4(1.0f), pPosition)
							* rotate(glm::mat4(1.0f), pRotation, { 0.0f, 0.0f, 1.0f })
							* scale(glm::mat4(1.0f), {pSize.x, pSize.y, 1.0f});
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}
}
