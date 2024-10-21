#include "EditorLayer.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"
#include "Owl/Debug/Instrumentor.h"
#include "Owl/Math/Math.h"
#include "Owl/Utils/PlatformUtils.h"

namespace OwlEditor
{
    EditorLayer::EditorLayer()
        : Layer("Sandbox2D"), m_CameraController(1280.0f / 780.0f)
    {
    }

    void EditorLayer::OnAttach()
    {
        OWL_PROFILE_FUNCTION();
	
        m_CheckerboardTexture = Texture2D::Create("Assets/Textures/Checkerboard.png");

        FramebufferSpecification framebufferSpecification;
    	framebufferSpecification.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        framebufferSpecification.Width = 1280;
        framebufferSpecification.Height = 780;
        m_Framebuffer = Framebuffer::Create(framebufferSpecification);

    	m_ActiveScene = CreateRef<Scene>();

    	m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

    	m_HierarchyPanel.SetContext(m_ActiveScene);

    	SceneSerializer sceneSerializer(m_ActiveScene);
    	sceneSerializer.Deserialize("Assets/Scenes/Example.owl");
		m_ActiveScenePath = "Assets/Scenes/Example.owl";
    }

    void EditorLayer::OnDetach()
    {
        OWL_PROFILE_FUNCTION();
	
    }

    void EditorLayer::OnUpdate(const DeltaTime pDeltaTime)
    {
    	OWL_PROFILE_FUNCTION();
    	
    	if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
    		m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
    		(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
    	{
    		m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    		m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
    		m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
    		m_ActiveScene->SetViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    	}
    	
    	if (m_ViewportFocused)
    	{
    		m_CameraController.OnUpdate(pDeltaTime);
    		m_EditorCamera.OnUpdate(pDeltaTime);
    	}


        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({.1f, .1f, .1f, 1});
        RenderCommand::Clear();

    	m_Framebuffer->ClearAttachment(1, -1); 

    	m_ActiveScene->OnUpdateEditor(pDeltaTime, m_EditorCamera);

    	auto [mx, my] = ImGui::GetMousePos();
    	mx -= m_ViewportBounds[0].x;
    	my -= m_ViewportBounds[0].y;
	    const auto viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
	    const int mouseX = static_cast<int>(mx);
	    const int mouseY = static_cast<int>(my);

    	if (mouseX >= 0 && mouseY >= 0 && mouseX < static_cast<int>(viewportSize.x) && mouseY < static_cast<int>(viewportSize.y))
    	{
    		int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
    			m_HoveredEntity = pixelData == -1 ? Entity() : Entity(static_cast<entt::entity>(pixelData), m_ActiveScene.get());
    	}
    	
    	m_Framebuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender()
    {
		OWL_PROFILE_FUNCTION();

		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
    	ImGuiStyle& style = ImGui::GetStyle();
    	float minWinSizeX = style.WindowMinSize.x;
    	style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
    	style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();
				
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();
				
				if (ImGui::MenuItem("Save", "Ctrl+S"))
					SaveScene();
				
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Stats");

		const std::string name = m_HoveredEntity ?  m_HoveredEntity.GetComponent<TagComponent>().Tag : "None";
    	ImGui::Text("Hovered Entity: %s", name.c_str());
    	
    	m_HierarchyPanel.OnImGuiRender();

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		const auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		const auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		const auto viewportOffset = ImGui::GetWindowPos();
    	m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
    	m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
    	
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererId();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    	
		// Gizmos
		if (Entity selectedEntity = m_HierarchyPanel.GetSelectedEntity(); selectedEntity && m_GizmoType != -1)
    	{
    		ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Camera

			// Runtime Camera
			// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			// const glm::mat4& cameraProjection = camera.GetProjection();
			// glm::mat4 cameraView = inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor Camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = transformComponent.GetTransform();

			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			Manipulate(value_ptr(cameraView), value_ptr(cameraProjection),
				static_cast<ImGuizmo::OPERATION>(m_GizmoType), ImGuizmo::LOCAL, value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
				Math::DecomposeTransform(transform, transformComponent.Translation, transformComponent.Rotation, transformComponent.Scale);
    	}
    	
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

    void EditorLayer::OnEvent(Event& pEvent)
    {
        m_CameraController.OnEvent(pEvent);
    	m_EditorCamera.OnEvent(pEvent);

    	EventDispatcher dispatcher(pEvent);
    	dispatcher.Dispatch<KeyPressedEvent>(OWL_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    	dispatcher.Dispatch<MouseButtonPressedEvent>(OWL_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
    }

    bool EditorLayer::OnKeyPressed(const KeyPressedEvent& pEvent)
    {
    	if (pEvent.GetRepeatCount() > 0)
    		return false;

    	bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
    	bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
    	switch (pEvent.GetKeyCode())
    	{
    	case Key::N:
    		{
    			if (control)
    				NewScene();
    			break;
    		}
    	case Key::O:
    		{
    			if (control)
    				OpenScene();
    			break;
    		}
    	case Key::S:
    		{
    			if (control)
    				SaveScene();
    			if (control && shift)
    				SaveSceneAs();
    			break;
    		}
    		// Gizmos
    	case Key::Q:
    		{
    			if (!ImGuizmo::IsUsing())
    				m_GizmoType = -1;
    			break;
    		}
    	case Key::W:
    		{
    			if (!ImGuizmo::IsUsing())
    				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
    			break;
    		}
    	case Key::E:
    		{
    			if (!ImGuizmo::IsUsing())
    				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
    			break;
    		}
    	case Key::R:
    		{
    			if (!ImGuizmo::IsUsing())
    				m_GizmoType = ImGuizmo::OPERATION::SCALE;
    			break;
    		}
    	}
    }

    bool EditorLayer::OnMouseButtonPressed(const MouseButtonPressedEvent& pEvent)
    {
    	if (pEvent.GetMouseButton() == Mouse::ButtonLeft)
    		if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
    			m_HierarchyPanel.SetSelectedEntity(m_HoveredEntity);

    	return false;
    }

    void EditorLayer::NewScene()
    {
    	m_ActiveScene = CreateRef<Scene>();
    	m_ActiveScene->SetViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
    	m_HierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OpenScene()
    {
    	if (const auto filepath = FileDialogs::OpenFile("Owl Scene (*.owl)\0*.owl\0"))
    	{
    		m_ActiveScene = CreateRef<Scene>();
    		m_ActiveScene->SetViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
    		m_HierarchyPanel.SetContext(m_ActiveScene);
					
    		SceneSerializer sceneSerializer(m_ActiveScene);
    		sceneSerializer.Deserialize(*filepath);
						
    		m_ActiveScenePath = *filepath;
    	}
    }

    void EditorLayer::SaveScene()
    {
    	if (!m_ActiveScenePath.empty())
    	{
    		SceneSerializer sceneSerializer(m_ActiveScene);
    		sceneSerializer.Serialize(m_ActiveScenePath);
    	}
    	else
    	{
    		SaveSceneAs();
    	}
    }

    void EditorLayer::SaveSceneAs()
    {
    	if (const auto filepath = FileDialogs::SaveFile("Owl Scene (*.owl)\0*.owl\0"))
    	{
    		SceneSerializer sceneSerializer(m_ActiveScene);
    		sceneSerializer.Serialize(*filepath);
    	}
    }
}
