#include "opch.h"
#include "Scene.h"

#include "Components.h"
#include "Owl/Renderer/Renderer2D.h"

namespace Owl
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    entt::entity Scene::CreateEntity()
    {
        return m_Registry.create();
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
