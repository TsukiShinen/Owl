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
        // Render sprites
        Camera* mainCamera = nullptr;
        glm::mat4* cameraTransform = nullptr;
        auto view = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto entity : view)
        {
            auto [transformComponent, cameraComponent] = view.get<TransformComponent, CameraComponent>(entity);
            if (cameraComponent.Primary)
            {
                mainCamera = &cameraComponent.Camera;
                cameraTransform = &transformComponent.Transform;
                break;
            }
        }

        if (mainCamera)
        {
            Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                auto [transformComponent, spriteComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transformComponent, spriteComponent.Color);
            }

            Renderer2D::EndScene();
        }
    }

    void Scene::SetViewportResize(uint32_t pWidth, uint32_t pHeight)
    {
        m_ViewportWidth = pWidth;
        m_ViewportHeight = pHeight;

        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (cameraComponent.FixedAspectRatio)
                continue;

            cameraComponent.Camera.SetViewportSize(pWidth, pHeight);
        }
    }
}
