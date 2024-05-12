#include "Sandbox2D.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 780.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Owl::Texture2D::Create("Assets/Textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	Layer::OnDetach();
}

void Sandbox2D::OnUpdate(const Owl::DeltaTime pDeltaTime)
{
	m_CameraController.OnUpdate(pDeltaTime);

	Owl::RenderCommand::SetClearColor({.1f, .1f, .1f, 1});
	Owl::RenderCommand::Clear();

	Owl::Renderer2D::BeginScene(m_CameraController.GetCamera());
	{
		Owl::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { .8f, .8f }, m_SquareColor);
		Owl::Renderer2D::DrawQuad({ 0.6f, -0.5f }, { .5f, .75f }, m_SquareColor);
		Owl::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { .5f, .5f }, m_CheckerboardTexture, m_SquareColor);
	}
	Owl::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	{
		ImGui::ColorEdit4("Square Color", value_ptr(m_SquareColor));
	}
	ImGui::End();
}

void Sandbox2D::OnEvent(Owl::Event& pEvent)
{
	m_CameraController.OnEvent(pEvent);
}
