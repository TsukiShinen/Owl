#pragma once


#include "Owl/Core/UUID.h"
#include "Owl/Renderer/Texture.h"
#include "Owl/Scene/SceneCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

namespace Owl
{
	struct IdComponent
	{
		Uuid Id;

		IdComponent() = default;
		IdComponent(const IdComponent&) = default;

		IdComponent(const Uuid pId)
			: Id(pId)
		{
		}
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;

		TagComponent(std::string pTag)
			: Tag(std::move(pTag))
		{
		}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
		glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
		glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		TransformComponent(const glm::vec3& pTranslation, const glm::vec3& pRotation = {0.0f, 0.0f, 0.0f},
		                   const glm::vec3& pScale = {0.0f, 0.0f, 0.0f})
			: Translation(pTranslation), Rotation(pRotation), Scale(pScale)
		{
		}

		[[nodiscard]] glm::mat4 GetTransform() const
		{
			const glm::mat4 rotation = toMat4(glm::quat(Rotation));

			return translate(glm::mat4(1.0f), Translation)
			       * rotation
			       * scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;

		SpriteRendererComponent(const glm::vec4& pColor)
			: Color(pColor)
		{
		}
	};

	struct CircleRendererComponent
	{
		glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; // TODO: think about moving to scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*Instantiate)();
		void (*Destroy)(NativeScriptComponent*);

		template <typename T>
		void Bind()
		{
			Instantiate = []() { return static_cast<ScriptableEntity*>(new T()); };
			Destroy = [](NativeScriptComponent* pNsc)
			{
				delete pNsc->Instance;
				pNsc->Instance = nullptr;
			};
		}
	};

	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };

		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = {0.0f, 0.0f};
		glm::vec2 Size = {0.5f, 0.5f};

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};
}
