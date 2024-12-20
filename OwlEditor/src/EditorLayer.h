﻿#pragma once
#include <Owl.h>

#include "Owl/Events/KeyEvent.h"
#include "Owl/Scene/Entity.h"
#include "Owl/Scene/SceneSerializer.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

namespace Owl
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(DeltaTime pDeltaTime) override;
		void OnImGuiRender() override;
		void OnEvent(Event& pEvent) override;

	private:
		bool OnKeyPressed(const KeyPressedEvent& pEvent);
		bool OnMouseButtonPressed(const MouseButtonPressedEvent& pEvent);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& pPath);
		void SaveScene();
		void SaveSceneAs();

		void OnScenePlay();
		void OnSceneStop();

		void OnDuplicateEntity();

		// Ui Panels
		void Ui_Toolbar();
		OrthographicCameraController m_CameraController;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;
		std::filesystem::path m_EditorScenePath;

		Entity m_HoveredEntity;

		EditorCamera m_EditorCamera;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = {0.0f, 0.0f};
		glm::vec2 m_ViewportBounds[2];

		Ref<Framebuffer> m_Framebuffer;

		int m_GizmoType = -1;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		enum class SceneState
		{
			Edit = 0, Play = 1
		};

		SceneState m_SceneState = SceneState::Edit;

		// Editor resources
		Ref<Texture2D> m_IconPlay, m_IconStop;
	};
}
