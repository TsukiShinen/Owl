#include "EditorLayer.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "Owl/Debug/Instrumentor.h"
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
        framebufferSpecification.Width = 1280;
        framebufferSpecification.Height = 780;
        m_Framebuffer = Framebuffer::Create(framebufferSpecification);

    	m_ActiveScene = CreateRef<Scene>();

#if 0
    	m_SquareEntity = m_ActiveScene->CreateEntity("Square");
    	m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

    	m_MainCameraEntity = m_ActiveScene->CreateEntity("Main Camera");
    	m_MainCameraEntity.AddComponent<CameraComponent>();

    	m_SecondCameraEntity= m_ActiveScene->CreateEntity("Second Camera");
    	auto& secondCameraComponent = m_SecondCameraEntity.AddComponent<CameraComponent>();
		secondCameraComponent.Primary = false;

    	class CameraController : public ScriptableEntity
    	{
    	public:
    		void OnUpdate(DeltaTime pDeltaTime) override
		    {
    			auto& translation = GetComponent<TransformComponent>().Translation;
    			float speed = 5.0f;

    			if (Input::IsKeyPressed(Key::A))
    				translation.x -= speed * pDeltaTime;
    			if (Input::IsKeyPressed(Key::D))
    				translation.x += speed * pDeltaTime;
    			if (Input::IsKeyPressed(Key::W))
    				translation.y += speed * pDeltaTime;
    			if (Input::IsKeyPressed(Key::S))
    				translation.y -= speed * pDeltaTime;
    		}
    	};
    	
    	m_MainCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

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

    	EventDispatcher dispatcher(pEvent);
    	dispatcher.Dispatch<KeyPressedEvent>(OWL_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
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
    	}
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
