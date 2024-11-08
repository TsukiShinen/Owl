﻿#pragma once

#include "entt.hpp"
#include "Owl/Core/DeltaTime.h"
#include "Owl/Core/UUID.h"
#include "Owl/Renderer/EditorCamera.h"

class b2World;

namespace Owl
{
	class Entity;

	class Scene
	{
	public:
		Scene() = default;
		~Scene();
		
		static Ref<Scene> Copy(Ref<Scene> pOther);

		Entity CreateEntity(const std::string& pName = std::string());
		Entity CreateEntityWithUuid(Uuid pUuid, const std::string& pName = std::string());
		void Destroy(Entity pEntity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateRuntime(DeltaTime pDeltaTime);
		void OnUpdateEditor(DeltaTime pDeltaTime, const EditorCamera& pCamera);
		void OnViewportResize(uint32_t pWidth, uint32_t pHeight);

		void DuplicateEntity(Entity pEntity);

		[[nodiscard]] Entity GetPrimaryCameraEntity();

	private:
		template <typename T>
		void OnComponentAdded(Entity pEntity, T& pComponent);
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
