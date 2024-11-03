#include "opch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"
#include "Owl/Renderer/Renderer2D.h"

#include "box2d/box2d.h"

namespace Owl
{
    static b2BodyType Rigidbody2DTypeToBox2DBody(const Rigidbody2DComponent::BodyType pBodyType)
    {
        switch (pBodyType)
        {
            case Rigidbody2DComponent::BodyType::Static: return b2_staticBody;
            case Rigidbody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
            case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
        }

        OWL_CORE_ASSERT(false, "Unknown body type")
        return b2_staticBody;
    }
    
    Scene::~Scene()
    {
        delete m_PhysicsWorld;
        m_PhysicsWorld = nullptr;
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

    void Scene::OnRuntimeStart()
    {
        m_PhysicsWorld = new b2World({ 0.0f, -9.8f });
        const auto view = m_Registry.view<Rigidbody2DComponent>();
        for (const auto e : view)
        {
            Entity entity = { e, this };
            const auto& transform = entity.GetComponent<TransformComponent>();
            auto& rigidbody2d = entity.GetComponent<Rigidbody2DComponent>();

            // Rigidbody
            b2BodyDef bodyDef;
            bodyDef.type = Rigidbody2DTypeToBox2DBody(rigidbody2d.Type);
            bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
            bodyDef.angle = transform.Rotation.z;
			
            b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
            body->SetFixedRotation(rigidbody2d.FixedRotation);
            rigidbody2d.RuntimeBody = body;

            // Box Collider
            if (entity.HasComponent<BoxCollider2DComponent>())
            {
                const auto& boxCollider2D = entity.GetComponent<BoxCollider2DComponent>();
                
                b2PolygonShape boxShape;
                boxShape.SetAsBox(boxCollider2D.Size.x * transform.Scale.x, boxCollider2D.Size.y * transform.Scale.y);
                
                b2FixtureDef fixtureDef;
                fixtureDef.shape = &boxShape;
                fixtureDef.density = boxCollider2D.Density;
                fixtureDef.friction = boxCollider2D.Friction;
                fixtureDef.restitution = boxCollider2D.Restitution;
                fixtureDef.restitutionThreshold = boxCollider2D.RestitutionThreshold;
                
                body->CreateFixture(&fixtureDef);
            }
        }
    }

    void Scene::OnRuntimeStop()
    {
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

        // Physics
        {
            constexpr int32_t velocityIterations = 6;
            constexpr int32_t positionIterations = 2;
            m_PhysicsWorld->Step(pDeltaTime, velocityIterations, positionIterations);
            
            // Retrieve transform from Box2D
            const auto view = m_Registry.view<Rigidbody2DComponent>();
            for (const auto e : view)
            {
                Entity entity = { e, this };
                auto& transform = entity.GetComponent<TransformComponent>();
                const auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
                
                const auto body = static_cast<b2Body*>(rb2d.RuntimeBody);
                const auto& position = body->GetPosition();
                transform.Translation.x = position.x;
                transform.Translation.y = position.y;
                transform.Rotation.z = body->GetAngle();
            }
        }
        
        // Render 2D
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

                Renderer2D::DrawSprite(transformComponent.GetTransform(), spriteComponent, static_cast<int>(entity));
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

            Renderer2D::DrawSprite(transformComponent.GetTransform(), spriteComponent, static_cast<int>(entity));
        }

        Renderer2D::EndScene();
    }

    void Scene::OnViewportResize(const uint32_t pWidth, const uint32_t pHeight)
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
        if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
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
    
    template <>
    void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity pEntity, Rigidbody2DComponent& pComponent)
    {
    }
    
    template <>
    void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity pEntity, BoxCollider2DComponent& pComponent)
    {
    }
}
