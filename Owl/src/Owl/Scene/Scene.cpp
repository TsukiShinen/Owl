#include "opch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"
#include "Owl/Renderer/Renderer2D.h"

namespace Owl
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& pName)
    {
        Entity entity{m_Registry.create(), this};
    	entity.AddComponent<TransformComponent>();
    	auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = pName.empty() ? "Entity" : pName;
        
        return entity;
    }

    void Scene::OnUpdate(DeltaTime pDeltaTime)
    {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawQuad(transform, sprite.Color);
        }
    }
}
