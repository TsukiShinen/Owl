#pragma once

#include "EditorCamera.h"
#include "OrthographicCamera.h"
#include "Texture.h"
#include "Owl/Scene/Components.h"

namespace Owl
{
	class Camera;

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& pCamera, const glm::mat4& pTransform);
		static void BeginScene(const EditorCamera& pCamera);
		static void BeginScene(const OrthographicCamera& pCamera); // TODO: Remove
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec2& pPosition, const glm::vec2& pSize, const glm::vec4& pColor);
		static void DrawQuad(const glm::vec3& pPosition, const glm::vec2& pSize, const glm::vec4& pColor);
		static void DrawQuad(const glm::vec2& pPosition, const glm::vec2& pSize, const Ref<Texture2D>& pTexture,
		                     float pTilingFactor, const glm::vec4& pColor = {1.0f, 1.0f, 1.0f, 1.0f});
		static void DrawQuad(const glm::vec3& pPosition, const glm::vec2& pSize, const Ref<Texture2D>& pTexture,
		                     float pTilingFactor, const glm::vec4& pTintColor = {1.0f, 1.0f, 1.0f, 1.0f});

		static void DrawQuad(const glm::mat4& pTransform, const glm::vec4& pColor, int pEntityId = -1);
		static void DrawQuad(const glm::mat4& pTransform, const Ref<Texture2D>& pTexture, float pTilingFactor = 1.0f,
		                     const glm::vec4& pTintColor = glm::vec4(1.0f), int pEntityId = -1);

		static void DrawRotatedQuad(const glm::vec2& pPosition, float pRotation, const glm::vec2& pSize,
		                            const glm::vec4& pColor);
		static void DrawRotatedQuad(const glm::vec3& pPosition, float pRotation, const glm::vec2& pSize,
		                            const glm::vec4& pColor);
		static void DrawRotatedQuad(const glm::vec2& pPosition, float pRotation, const glm::vec2& pSize,
		                            const Ref<Texture2D>& pTexture, float pTilingFactor,
		                            const glm::vec4& pColor = {1.0f, 1.0f, 1.0f, 1.0f});
		static void DrawRotatedQuad(const glm::vec3& pPosition, float pRotation, const glm::vec2& pSize,
		                            const Ref<Texture2D>& pTexture, float pTilingFactor,
		                            const glm::vec4& pTintColor = {1.0f, 1.0f, 1.0f, 1.0f});

		static void DrawCircle(const glm::mat4& pTransform, const glm::vec4& pColor, float pThickness = 1.0f, float pFade = 0.005f, int pEntityId = -1);

		static void DrawLine(const glm::vec3& pPoint0, const glm::vec3& pPoint1, const glm::vec4& pColor, int pEntityId = -1);
		
		static void DrawRect(const glm::vec3& pPosition, const glm::vec2& pSize, const glm::vec4& pColor, int pEntityId = -1);
		static void DrawRect(const glm::mat4& pTransform, const glm::vec4& pColor, int pEntityId = -1);
		
		static void DrawSprite(const glm::mat4& pTransform, const SpriteRendererComponent& pSpriteRendererComponent,
		                       int pEntityId);

		static float GetLineWidth();
		static void SetLineWidth(float pWidth);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static Statistics GetStats();
		static void ResetStats();

	private:
		static void StartBatch();
		static void NextBatch();
	};
}
