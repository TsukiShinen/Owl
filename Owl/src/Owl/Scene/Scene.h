#pragma once

#include "entt.hpp"
#include "Owl/Core/DeltaTime.h"

namespace Owl
{
    class Scene
    {
    public:
        Scene();
        ~Scene();
        
		entt::registry& Reg() { return m_Registry; }

        entt::entity CreateEntity();

        void OnUpdate(DeltaTime pDeltaTime);
    private:
        entt::registry m_Registry;
    };
}
