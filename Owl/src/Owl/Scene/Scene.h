#pragma once

#include "entt.hpp"
#include "Owl/Core/DeltaTime.h"

namespace Owl
{
    class Entity;
    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& pName = std::string());

        void OnUpdate(DeltaTime pDeltaTime);
    private:
        entt::registry m_Registry;

        friend class Entity;
    };
}
