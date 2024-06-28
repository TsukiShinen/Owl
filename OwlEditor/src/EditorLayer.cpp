﻿#include "EditorLayer.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "Owl/Debug/Instrumentor.h"

namespace Owl
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
        framebufferSpecification.Width = 1280;
        framebufferSpecification.Height = 780;
        m_Framebuffer = Framebuffer::Create(framebufferSpecification);

    	m_ActiveScene = CreateRef<Scene>();

    	m_SquareEntity = m_ActiveScene->CreateEntity("Square");
    	m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

    	m_MainCameraEntity = m_ActiveScene->CreateEntity("Camera");
    	m_MainCameraEntity.AddComponent<CameraComponent>();

    	m_SecondCameraEntity= m_ActiveScene->CreateEntity("Clip-Space");
    	auto& secondCameraComponent = m_SecondCameraEntity.AddComponent<CameraComponent>();
		secondCameraComponent.Primary = false;

    	class CameraController : public ScriptableEntity
    	{
    	public:
    		void OnCreate()
    		{
    			
    		}

    		void OnDestroy()
    		{
    			
    		}

    		void OnUpdate(DeltaTime pDeltaTime)
    		{
    			auto& transform = GetComponent<TransformComponent>().Transform;
    			float speed = 5.0f;

    			if (Input::IsKeyPressed(Key::A))
    				transform[3][0] -= speed * pDeltaTime;
    			if (Input::IsKeyPressed(Key::D))
    				transform[3][0] += speed * pDeltaTime;
    			if (Input::IsKeyPressed(Key::W))
    				transform[3][1] += speed * pDeltaTime;
    			if (Input::IsKeyPressed(Key::S))
    				transform[3][1] -= speed * pDeltaTime;
    		}
    	};
    	
    	m_MainCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
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

    		m_ActiveScene->SetViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    	}
    	
    	if (m_ViewportFocused)
			m_CameraController.OnUpdate(pDeltaTime);

        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({.1f, .1f, .1f, 1});
        RenderCommand::Clear();

    	m_ActiveScene->OnUpdate(pDeltaTime);
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
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		if (m_SquareEntity)
		{
    		ImGui::Separator();
			ImGui::Text("%s", m_SquareEntity.GetComponent<TagComponent>().Tag.c_str());
			auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
		}

		{
    		ImGui::Separator();
    		if (ImGui::Checkbox("Camera A", &m_PrimaryCamera))
    		{
    			m_MainCameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
    			m_SecondCameraEntity.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
    		}
		}

		{
    		ImGui::Separator();
			auto& camera = m_SecondCameraEntity.GetComponent<CameraComponent>().Camera;
			float orthoSize = camera.GetOrthographicSize();
			if (ImGui::DragFloat("Camera Size", &orthoSize))
				camera.SetOrthographicSize(orthoSize);
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererId();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

    void EditorLayer::OnEvent(Event& pEvent)
    {
        m_CameraController.OnEvent(pEvent);
    }
}
