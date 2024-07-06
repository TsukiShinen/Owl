﻿#pragma once

#include "entt.hpp"
#include "Owl/Core/DeltaTime.h"

namespace OwlEditor
{
    class SceneHierarchyPanel;
}

namespace Owl
{
    class Entity;
    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& pName = std::string());
        void Destroy(Entity pEntity);

        void OnUpdate(DeltaTime pDeltaTime);
        void SetViewportResize(uint32_t pWidth, uint32_t pHeight);
    private:
        template<typename T>
        void OnComponentAdded(Entity pEntity, T& pComponent);
    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        friend class Entity;
        friend class OwlEditor::SceneHierarchyPanel;
    };
}