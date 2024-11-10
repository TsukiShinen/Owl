#include "opch.h"
#include "Renderer2D.h"

#include "Camera.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "UniformBuffer.h"
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

		// Editor-Only
		int EntityId = 0;
	};
	
	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		// Editor-Only
		int EntityId = 0;
	};
	
	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Editor-Only
		int EntityId = 0;
	};

	struct Renderer2DData
	{
		static constexpr uint32_t k_MaxQuads = 20000;
		static constexpr uint32_t k_MaxVertices = k_MaxQuads * 4;
		static constexpr uint32_t k_MaxIndices = k_MaxQuads * 6;
		static constexpr uint32_t k_MaxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexture;
		
		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;
		
		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
		
		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;
		
		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		float LineWidth = 2.0f;

		std::array<Ref<Texture2D>, k_MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = WhiteTexture

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};

		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		OWL_PROFILE_FUNCTION();

		// Quads
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.k_MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "in_Position"},
			{ShaderDataType::Float4, "in_Color"},
			{ShaderDataType::Float2, "in_TexCoord"},
			{ShaderDataType::Float, "in_TexIndex"},
			{ShaderDataType::Float, "in_TilingFactor"},
			{ShaderDataType::Int, "in_EntityId"}
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.k_MaxVertices];

		auto quadIndices = new uint32_t[s_Data.k_MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.k_MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data.k_MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIndexBuffer);
		delete[] quadIndices;

		// Circles
		s_Data.CircleVertexArray = VertexArray::Create();

		s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.k_MaxVertices * sizeof(CircleVertex));
		s_Data.CircleVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "in_WorldPosition"},
			{ShaderDataType::Float3, "in_LocalPosition"},
			{ShaderDataType::Float4, "in_Color"},
			{ShaderDataType::Float, "in_Thickness"},
			{ShaderDataType::Float, "in_Fade"},
			{ShaderDataType::Int, "in_EntityId"}
		});
		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
		s_Data.CircleVertexArray->SetIndexBuffer(quadIndexBuffer);
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.k_MaxVertices];

		// Lines
		s_Data.LineVertexArray = VertexArray::Create();

		s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.k_MaxVertices * sizeof(LineVertex));
		s_Data.LineVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "in_Position"},
			{ShaderDataType::Float4, "in_Color"},
			{ShaderDataType::Int, "in_EntityId"}
		});
		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.k_MaxVertices];

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		auto whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int samplers[Renderer2DData::k_MaxTextureSlots];
		for (auto i = 0; i < s_Data.k_MaxTextureSlots; ++i)
			samplers[i] = i;

		s_Data.QuadShader = Shader::Create("Assets/Shaders/Renderer2D_Quad.glsl");
		s_Data.CircleShader = Shader::Create("Assets/Shaders/Renderer2D_Circle.glsl");
		s_Data.LineShader = Shader::Create("Assets/Shaders/Renderer2D_Line.glsl");

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
		s_Data.QuadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
		s_Data.QuadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
		s_Data.QuadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		OWL_PROFILE_FUNCTION();

		delete [] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& pCamera, const glm::mat4& pTransform)
	{
		OWL_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = pCamera.GetProjection() * inverse(pTransform);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& pCamera)
	{
		OWL_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = pCamera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& pCamera)
	{
		OWL_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = pCamera.GetViewProjectionMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		OWL_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount)
		{
			const auto dataSize = static_cast<uint32_t>(
			reinterpret_cast<uint8_t*>(s_Data.QuadVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_Data.
				QuadVertexBufferBase));
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; ++i)
				s_Data.TextureSlots[i]->Bind(i);

			s_Data.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.CircleIndexCount)
		{
			const auto dataSize = static_cast<uint32_t>(
			reinterpret_cast<uint8_t*>(s_Data.CircleVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_Data.
				CircleVertexBufferBase));
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);
			
			s_Data.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.LineVertexCount)
		{
			const auto dataSize = static_cast<uint32_t>(
			reinterpret_cast<uint8_t*>(s_Data.LineVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_Data.
				LineVertexBufferBase));
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);
			
			s_Data.LineShader->Bind();
			RenderCommand::SetLineWidth(s_Data.LineWidth);
			RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
			s_Data.Stats.DrawCalls++;
		}
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		
		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

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
		glm::mat4 transform = translate(glm::mat4(1.0f), pPosition)
		                      * scale(glm::mat4(1.0f), {pSize.x, pSize.y, 1.0f});

		DrawQuad(transform, pColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pPosition, const glm::vec2& pSize, const Ref<Texture2D>& pTexture,
	                          const float pTilingFactor, const glm::vec4& pColor)
	{
		DrawQuad({pPosition.x, pPosition.y, 0.0f}, pSize, pTexture, pTilingFactor, pColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pPosition, const glm::vec2& pSize, const Ref<Texture2D>& pTexture,
	                          const float pTilingFactor, const glm::vec4& pTintColor)
	{
		glm::mat4 transform = translate(glm::mat4(1.0f), pPosition)
		                      * scale(glm::mat4(1.0f), {pSize.x, pSize.y, 1.0f});

		DrawQuad(transform, pTexture, pTilingFactor, pTintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& pTransform, const glm::vec4& pColor, int pEntityId)
	{
		OWL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr auto textureIndex = 0.0f;
		constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
		constexpr auto tilingFactor = 1.0f;

		if (s_Data.QuadIndexCount >= Renderer2DData::k_MaxIndices)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = pTransform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = pColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityId = pEntityId;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& pTransform, const Ref<Texture2D>& pTexture, float pTilingFactor,
	                          const glm::vec4& pTintColor, int pEntityId)
	{
		OWL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

		if (s_Data.QuadIndexCount >= Renderer2DData::k_MaxIndices)
			NextBatch();

		auto textureIndex = 0.0f;
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
			if (s_Data.TextureSlotIndex >= Renderer2DData::k_MaxTextureSlots)
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
			s_Data.QuadVertexBufferPtr->EntityId = pEntityId;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pPosition, float pRotation, const glm::vec2& pSize,
	                                 const glm::vec4& pColor)
	{
		DrawRotatedQuad({pPosition.x, pPosition.y, 0}, pRotation, pSize, pColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pPosition, float pRotation, const glm::vec2& pSize,
	                                 const glm::vec4& pColor)
	{
		OWL_PROFILE_FUNCTION();

		glm::mat4 transform = translate(glm::mat4(1.0f), pPosition)
		                      * rotate(glm::mat4(1.0f), glm::radians(pRotation), {0.0f, 0.0f, 1.0f})
		                      * scale(glm::mat4(1.0f), {pSize.x, pSize.y, 1.0f});

		DrawQuad(transform, pColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pPosition, float pRotation, const glm::vec2& pSize,
	                                 const Ref<Texture2D>& pTexture, const float pTilingFactor, const glm::vec4& pColor)
	{
		DrawRotatedQuad({pPosition.x, pPosition.y, 0}, pRotation, pSize, pTexture, pTilingFactor, pColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pPosition, float pRotation, const glm::vec2& pSize,
	                                 const Ref<Texture2D>& pTexture, float pTilingFactor, const glm::vec4& pTintColor)
	{
		OWL_PROFILE_FUNCTION();

		glm::mat4 transform = translate(glm::mat4(1.0f), pPosition)
		                      * rotate(glm::mat4(1.0f), glm::radians(pRotation), {0.0f, 0.0f, 1.0f})
		                      * scale(glm::mat4(1.0f), {pSize.x, pSize.y, 1.0f});

		DrawQuad(transform, pTexture, pTilingFactor, pTintColor);
	}

	void Renderer2D::DrawCircle(const glm::mat4& pTransform, const glm::vec4& pColor, float pThickness, float pFade,
		int pEntityId)
	{
		OWL_PROFILE_FUNCTION();
		
		// TODO: Batch for Circles
		
		for (size_t i = 0; i < 4; i++)
		{
			s_Data.CircleVertexBufferPtr->WorldPosition = pTransform * s_Data.QuadVertexPositions[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Color = pColor;
			s_Data.CircleVertexBufferPtr->Thickness = pThickness;
			s_Data.CircleVertexBufferPtr->Fade = pFade;
			s_Data.CircleVertexBufferPtr->EntityId = pEntityId;
			s_Data.CircleVertexBufferPtr++;
		}
		s_Data.CircleIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawLine(const glm::vec3& pPoint0, const glm::vec3& pPoint1, const glm::vec4& pColor,
		int pEntityId)
	{
		OWL_PROFILE_FUNCTION();

		// TODO: Batch for Lines

		s_Data.LineVertexBufferPtr->Position = pPoint0;
		s_Data.LineVertexBufferPtr->Color = pColor;
		s_Data.LineVertexBufferPtr->EntityId = pEntityId;
		s_Data.LineVertexBufferPtr++;
		
		s_Data.LineVertexBufferPtr->Position = pPoint1;
		s_Data.LineVertexBufferPtr->Color = pColor;
		s_Data.LineVertexBufferPtr->EntityId = pEntityId;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& pPosition, const glm::vec2& pSize, const glm::vec4& pColor,
		int pEntityId)
	{
		glm::vec3 p0 = glm::vec3(pPosition.x - pSize.x * 0.5f, pPosition.y - pSize.y * 0.5f, pPosition.z);
		glm::vec3 p1 = glm::vec3(pPosition.x + pSize.x * 0.5f, pPosition.y - pSize.y * 0.5f, pPosition.z);
		glm::vec3 p2 = glm::vec3(pPosition.x + pSize.x * 0.5f, pPosition.y + pSize.y * 0.5f, pPosition.z);
		glm::vec3 p3 = glm::vec3(pPosition.x - pSize.x * 0.5f, pPosition.y + pSize.y * 0.5f, pPosition.z);
		
		DrawLine(p0, p1, pColor, pEntityId);
		DrawLine(p1, p2, pColor, pEntityId);
		DrawLine(p2, p3, pColor, pEntityId);
		DrawLine(p3, p0, pColor, pEntityId);
	}

	void Renderer2D::DrawRect(const glm::mat4& pTransform, const glm::vec4& pColor, int pEntityId)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = pTransform * s_Data.QuadVertexPositions[i];
		
		DrawLine(lineVertices[0], lineVertices[1], pColor, pEntityId); 
		DrawLine(lineVertices[1], lineVertices[2], pColor, pEntityId);
		DrawLine(lineVertices[2], lineVertices[3], pColor, pEntityId);
		DrawLine(lineVertices[3], lineVertices[0], pColor, pEntityId);
	}

	void Renderer2D::DrawSprite(const glm::mat4& pTransform, const SpriteRendererComponent& pSpriteRendererComponent,
	                            const int pEntityId)
	{
		if (pSpriteRendererComponent.Texture)
		{
			DrawQuad(pTransform, pSpriteRendererComponent.Texture, pSpriteRendererComponent.TilingFactor,
			         pSpriteRendererComponent.Color, pEntityId);
		}
		else
			DrawQuad(pTransform, pSpriteRendererComponent.Color, pEntityId);
	}

	float Renderer2D::GetLineWidth()
	{
		return s_Data.LineWidth;
	}

	void Renderer2D::SetLineWidth(const float pWidth)
	{
		s_Data.LineWidth = pWidth;
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
