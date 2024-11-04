#include "opch.h"
#include "Entity.h"

namespace Owl
{
	Entity::Entity(const entt::entity pHandle, Scene* pScene)
		: m_EntityHandle(pHandle), m_Scene(pScene)
	{
	}
}
