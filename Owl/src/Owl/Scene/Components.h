﻿#pragma once

#include <glm/glm.hpp>

#include "Owl/Renderer/Camera.h"

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
        Camera Camera;
        bool Primary = true; // TODO: think about moving to scene
        
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
        CameraComponent(const glm::mat4& pProjection)
            : Camera(pProjection) {}
    };
}
