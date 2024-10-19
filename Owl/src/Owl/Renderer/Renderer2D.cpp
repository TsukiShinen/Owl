#include "opch.h"
#include "Renderer2D.h"

#include "Camera.h"
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
		float TexIndex;
		float TilingFactor;
	};
	
	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps
		
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Texture2D> WhiteTexture;
		Ref<Shader> TextureShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = WhiteTexture

		glm::vec4 QuadVertexPositions[4];
		
		Renderer2D::Statistics Stats;
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
			{ShaderDataType::Float2, "in_TexCoord"},
			{ShaderDataType::Float, "in_TexIndex"},
			{ShaderDataType::Float, "in_TilingFactor"}
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

		int samplers[Renderer2DData::MaxTextureSlots];
		for (int i = 0; i < s_Data.MaxTextureSlots; ++i)
			samplers[i] = i;
		
		s_Data.TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Texture", samplers);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
		
		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		OWL_PROFILE_FUNCTION();

		delete [] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& pCamera, const glm::mat4& pTransform)
	{
		OWL_PROFILE_FUNCTION();

		const glm::mat4 viewProj = pCamera.GetProjection() * glm::inverse(pTransform);
		
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& pCamera)
	{
		OWL_PROFILE_FUNCTION();

		const glm::mat4 viewProj = pCamera.GetViewProjection();
		
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		StartBatch();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& pCamera)
	{
		OWL_PROFILE_FUNCTION();
		
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", pCamera.GetViewProjectionMatrix());

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		OWL_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount == 0)
			return;

		const uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(s_Data.QuadVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_Data.
			QuadVertexBufferBase));
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
		
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; ++i)
			s_Data.TextureSlots[i]->Bind(i);
		
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}
	
	void Renderer2D::DrawQuad(const glm::vec2& pPosition, const glm::vec2& pSize, const glm::vec4& pColor)
	{
		DrawQuad({pPosition.x, pPosition.y, 0.0f}, pSize, pColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pPosition, const glm::vec2& pSize, const glm::vec4& pColor)
	{
		OWL_PROFILE_FUNCTION();

		glm::mat4 transform = translate(glm::mat4(1.0f), pPosition)
							* scale(glm::mat4(1.0f), { pSize.x, pSize.y, 1.0f });

		DrawQuad(transform, pColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pPosition, const glm::vec2& pSize, const Ref<Texture2D>& pTexture, const float pTilingFactor, const glm::vec4& pColor)
	{
		DrawQuad({pPosition.x, pPosition.y, 0.0f}, pSize, pTexture, pTilingFactor, pColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pPosition, const glm::vec2& pSize, const Ref<Texture2D>& pTexture, const float pTilingFactor, const glm::vec4& pTintColor)
	{
		OWL_PROFILE_FUNCTION();

		glm::mat4 transform = translate(glm::mat4(1.0f), pPosition)
							* scale(glm::mat4(1.0f), { pSize.x, pSize.y, 1.0f });

		DrawQuad(transform, pTexture, pTilingFactor, pTintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& pTransform, const glm::vec4& pColor)
	{
		constexpr  size_t quadVertexCount = 4;
		constexpr float textureIndex = 0.0f;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr float tilingFactor = 1.0f;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = pTransform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = pColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& pTransform, const Ref<Texture2D>& pTexture, float pTilingFactor,
		const glm::vec4& pTintColor)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
		{
			if (*s_Data.TextureSlots[i].get() == *pTexture.get())
			{
				textureIndex = static_cast<float>(i);
				break;
			}
		}
		
		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();
			
			textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = pTexture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = pTransform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = pTintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = pTilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pPosition, float pRotation, const glm::vec2& pSize, const glm::vec4& pColor)
	{
		DrawRotatedQuad({pPosition.x, pPosition.y, 0}, pRotation, pSize, pColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pPosition, float pRotation, const glm::vec2& pSize, const glm::vec4& pColor)
	{
		OWL_PROFILE_FUNCTION();

		glm::mat4 transform = translate(glm::mat4(1.0f), pPosition)
							* rotate(glm::mat4(1.0f), glm::radians(pRotation), { 0.0f, 0.0f, 1.0f})
							* scale(glm::mat4(1.0f), { pSize.x, pSize.y, 1.0f });

		DrawQuad(transform, pColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pPosition, float pRotation, const glm::vec2& pSize, const Ref<Texture2D>& pTexture, const float pTilingFactor, const glm::vec4& pColor)
	{
		DrawRotatedQuad({pPosition.x, pPosition.y, 0}, pRotation, pSize, pTexture, pTilingFactor, pColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pPosition, float pRotation, const glm::vec2& pSize, const Ref<Texture2D>& pTexture, float pTilingFactor, const glm::vec4& pTintColor)
	{
		OWL_PROFILE_FUNCTION();
		
		glm::mat4 transform = translate(glm::mat4(1.0f), pPosition)
							* rotate(glm::mat4(1.0f), glm::radians(pRotation), { 0.0f, 0.0f, 1.0f})
							* scale(glm::mat4(1.0f), { pSize.x, pSize.y, 1.0f });

		DrawQuad(transform, pTexture, pTilingFactor, pTintColor);
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}
}
