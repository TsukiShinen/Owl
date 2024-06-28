#pragma once

#include <glm/glm.hpp>

#include "ScriptableEntity.h"
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
        glm::mat4 Transform{ 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& pTransform)
            : Transform(pTransform) {}

        operator glm::mat4& () { return Transform; }
        operator const glm::mat4& () const { return Transform; }
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
