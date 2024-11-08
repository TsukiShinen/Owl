﻿#include "Sandbox2D.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "Owl/Debug/Instrumentor.h"

EditorLayer::EditorLayer()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 780.0f)
{
}

void EditorLayer::OnAttach()
{
	OWL_PROFILE_FUNCTION();
	
	m_CheckerboardTexture = Owl::Texture2D::Create("Assets/Textures/Checkerboard.png");
	m_ChernoTexture = Owl::Texture2D::Create("Assets/Textures/ChernoLogo.png");
}

void EditorLayer::OnDetach()
{
	OWL_PROFILE_FUNCTION();
	
	Layer::OnDetach();
}

void EditorLayer::OnUpdate(const Owl::DeltaTime pDeltaTime)
{
	OWL_PROFILE_FUNCTION();
	m_CameraController.OnUpdate(pDeltaTime);

	Owl::Renderer2D::ResetStats();
	{
		OWL_PROFILE_SCOPE("Render Preparation");
		
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
	}
}

void EditorLayer::OnImGuiRender()
{
	OWL_PROFILE_FUNCTION();
	
	ImGui::Begin("Settings");

	auto stats = Owl::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();
}

void EditorLayer::OnEvent(Owl::Event& pEvent)
{
	m_CameraController.OnEvent(pEvent);
}
