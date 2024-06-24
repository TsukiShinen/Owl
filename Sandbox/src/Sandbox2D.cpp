#include "Sandbox2D.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "Owl/Debug/Instrumentor.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 780.0f)
{
}

void Sandbox2D::OnAttach()
{
	OWL_PROFILE_FUNCTION();
	
	m_CheckerboardTexture = Owl::Texture2D::Create("Assets/Textures/Checkerboard.png");
	m_ChernoTexture = Owl::Texture2D::Create("Assets/Textures/ChernoLogo.png");

	Owl::FramebufferSpecification framebufferSpecification;
	framebufferSpecification.Width = 1280;
	framebufferSpecification.Height = 780;
	m_Framebuffer = Owl::Framebuffer::Create(framebufferSpecification);
}

void Sandbox2D::OnDetach()
{
	OWL_PROFILE_FUNCTION();
	
	Layer::OnDetach();
}

void Sandbox2D::OnUpdate(const Owl::DeltaTime pDeltaTime)
{
	OWL_PROFILE_FUNCTION();
	m_CameraController.OnUpdate(pDeltaTime);

	Owl::Renderer2D::ResetStats();
	{
		OWL_PROFILE_SCOPE("Render Preparation");
		m_Framebuffer->Bind();
		
		Owl::RenderCommand::SetClearColor({.1f, .1f, .1f, 1});
		Owl::RenderCommand::Clear();
	}

	{
		OWL_PROFILE_SCOPE("Render Draw");
		
		Owl::Renderer2D::BeginScene(m_CameraController.GetCamera());
		{
			Owl::Renderer2D::DrawRotatedQuad({1.0f, 0.0f}, m_SquareRotation, {.8f, .8f}, m_SquareColor);
			Owl::Renderer2D::DrawQuad({-1.0f, -0.0f}, {.8f, .8f}, m_SquareColor);
			Owl::Renderer2D::DrawQuad({0.5f, -0.5f}, {.5f, .75f}, m_SquareColor);
			Owl::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {20.0f, 20.0f}, m_CheckerboardTexture, 20.0f);
			Owl::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, m_SquareRotation, { 1.0f, 1.0f }, m_CheckerboardTexture, 20.0f);
		}
		Owl::Renderer2D::EndScene();
		
		Owl::Renderer2D::BeginScene(m_CameraController.GetCamera());
		{
			for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.5f)
				{
					glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, .5f };
					Owl::Renderer2D::DrawQuad({x, y}, {.45f, .45f}, color);
				}
			}
		}
		Owl::Renderer2D::EndScene();
		m_Framebuffer->Unbind();
	}
}

void Sandbox2D::OnImGuiRender()
{
	OWL_PROFILE_FUNCTION();
	
	static bool dockingEnabled = true;
	if (dockingEnabled)
	{
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

				if (ImGui::MenuItem("Exit")) Owl::Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto stats = Owl::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererId();
		ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f });
		ImGui::End();

		ImGui::End();
	}
	else
	{
		ImGui::Begin("Settings");

		auto stats = Owl::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererId();
		ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f });
		ImGui::End();
	}
}

void Sandbox2D::OnEvent(Owl::Event& pEvent)
{
	m_CameraController.OnEvent(pEvent);
}
