#pragma once

#include <glm/glm.hpp>

#include "ScriptableEntity.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Owl/Scene/SceneCamera.h"

namespace Owl
{
    struct TagComponent
    {
        std::string Tag;
        
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(std::string pTag)
            : Tag(std::move(pTag)) {}
    };
    
    struct TransformComponent
    {
        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f};
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f};
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f};

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& pTranslation, const glm::vec3& pRotation = { 0.0f, 0.0f, 0.0f}, const glm::vec3& pScale = { 0.0f, 0.0f, 0.0f})
            : Translation(pTranslation), Rotation(pRotation), Scale(pScale) {}

        [[nodiscard]] glm::mat4 GetTransform() const
        {
            const glm::mat4 rotation = rotate(glm::mat4(1.0f), Rotation.x, {1, 0, 0})
                               * rotate(glm::mat4(1.0f), Rotation.y, {0, 1, 0})
                               * rotate(glm::mat4(1.0f), Rotation.z, {0, 0, 1});

            return translate(glm::mat4(1.0f), Translation)
                * rotation
                * scale(glm::mat4(1.0f), Scale);
        }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& pColor)
            : Color(pColor) {}
    };

    struct CameraComponent
    {
        SceneCamera Camera;
        bool Primary = true; // TODO: think about moving to scene
        bool FixedAspectRatio = false; 
        
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    struct NativeScriptComponent
    {
        ScriptableEntity* Instance = nullptr;
        
        ScriptableEntity*(*Instantiate)();
        void(*Destroy)(NativeScriptComponent*);

        template<typename T>
        void Bind()
        {
            Instantiate = []() { return static_cast<ScriptableEntity*>(new T()); };
            Destroy = [](NativeScriptComponent* pNsc) { delete pNsc->Instance; pNsc->Instance = nullptr; };
        }
    };
}
