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

    void Scene::Destroy(Entity pEntity)
    {
        m_Registry.destroy(pEntity);
    }

    void Scene::OnUpdateRuntime(DeltaTime pDeltaTime)
    {
        // Update scripts
        {
            m_Registry.view<NativeScriptComponent>().each([&](auto pEntity, auto& pNsc)
            {
                if (!pNsc.Instance)
                {
                    pNsc.Instance = pNsc.Instantiate();
                    pNsc.Instance->m_Entity = { pEntity, this };
                    pNsc.Instance->OnCreate();
                }
                pNsc.Instance->OnUpdate(pDeltaTime);
            });
        }
        
        // Render sprites
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        auto view = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto entity : view)
        {
            auto [transformComponent, cameraComponent] = view.get<TransformComponent, CameraComponent>(entity);
            if (cameraComponent.Primary)
            {
                mainCamera = &cameraComponent.Camera;
                cameraTransform = transformComponent.GetTransform();
                break;
            }
        }

        if (mainCamera)
        {
            Renderer2D::BeginScene(*mainCamera, cameraTransform);

            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                auto [transformComponent, spriteComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transformComponent.GetTransform(), spriteComponent.Color);
            }

            Renderer2D::EndScene();
        }
    }

    void Scene::OnUpdateEditor(DeltaTime pDeltaTime, const EditorCamera& pCamera)
    {
        Renderer2D::BeginScene(pCamera);

        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto [transformComponent, spriteComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawQuad(transformComponent.GetTransform(), spriteComponent.Color);
        }

        Renderer2D::EndScene();
    }

    void Scene::SetViewportResize(const uint32_t pWidth, const uint32_t pHeight)
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

    Entity Scene::GetPrimaryCameraEntity()
    {
        for (const auto view = m_Registry.view<CameraComponent>(); const auto entity : view)
        {
            if (const auto& camera = view.get<CameraComponent>(entity); camera.Primary)
                return Entity{entity, this};
        }
        return {};
    }

    template <typename T>
    void Scene::OnComponentAdded(Entity pEntity, T& pComponent)
    {
		static_assert(sizeof(T) == 0);
    }
    
    template <>
    void Scene::OnComponentAdded<TransformComponent>(Entity pEntity, TransformComponent& pComponent)
    {
    }
    
    template <>
    void Scene::OnComponentAdded<TagComponent>(Entity pEntity, TagComponent& pComponent)
    {
    }
    
    template <>
    void Scene::OnComponentAdded<CameraComponent>(Entity pEntity, CameraComponent& pComponent)
    {
        pComponent.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
    }
    
    template <>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity pEntity, SpriteRendererComponent& pComponent)
    {
    }
    
    template <>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity pEntity, NativeScriptComponent& pComponent)
    {
    }
}
