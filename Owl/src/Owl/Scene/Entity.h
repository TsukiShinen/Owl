#pragma once
#include <entt.hpp>

#include "Components.h"
#include "Scene.h"
#include "Owl/Core/UUID.h"

namespace Owl
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity pHandle, Scene* pScene);
        Entity(const Entity& pEntity) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... pArgs)
        {
            OWL_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!")
            T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(pArgs)...);
            m_Scene->OnComponentAdded<T>(*this, component);
            return component;
        }

        template<typename T>
        T& GetComponent()
        {
            OWL_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!")
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }
        
        template<typename T>
        [[nodiscard]] bool HasComponent() const
        {
            return m_Scene->m_Registry.has<T>(m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent() const
        {
            OWL_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!")
            return m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator entt::entity() const { return m_EntityHandle; }
        operator uint32_t() const { return static_cast<uint32_t>(m_EntityHandle); }

        Uuid GetUuid() { return GetComponent<IdComponent>().Id; }
        
        bool operator ==(const Entity& pOther) const { return m_EntityHandle == pOther.m_EntityHandle && m_Scene == pOther.m_Scene; }
        bool operator !=(const Entity& pOther) const { return !(*this == pOther); }
    private:
        entt::entity m_EntityHandle{entt::null};
        Scene* m_Scene = nullptr;
    };
}
