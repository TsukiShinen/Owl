#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"

namespace Owl
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& pCamera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& pPosition, const glm::vec2& pSize, const glm::vec4& pColor);
		static void DrawQuad(const glm::vec3& pPosition, const glm::vec2& pSize, const glm::vec4& pColor);
		static void DrawQuad(const glm::vec2& pPosition, const glm::vec2& pSize, const Ref<Texture2D>& pTexture, const glm::vec4& pColor = {1.0f, 1.0f, 1.0f, 1.0f});
		static void DrawQuad(const glm::vec3& pPosition, const glm::vec2& pSize, const Ref<Texture2D>& pTexture, const glm::vec4& pColor = {1.0f, 1.0f, 1.0f, 1.0f});
	};
	
}
