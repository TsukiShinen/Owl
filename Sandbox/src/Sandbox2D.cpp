﻿#include "Sandbox2D.h"

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

	{
		OWL_PROFILE_SCOPE("Render Preparation");
		
		Owl::RenderCommand::SetClearColor({.1f, .1f, .1f, 1});
		Owl::RenderCommand::Clear();
	}

	{
		OWL_PROFILE_SCOPE("Render Draw");
		
		Owl::Renderer2D::BeginScene(m_CameraController.GetCamera());
		{
			Owl::Renderer2D::DrawRotatedQuad({-1.0f, 0.0f}, glm::radians(m_SquareRotation), {.8f, .8f}, m_SquareColor);
			Owl::Renderer2D::DrawQuad({0.6f, -0.5f}, {.5f, .75f}, m_SquareColor);
			Owl::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {10.0f, 10.0f}, m_CheckerboardTexture, 10.0f);
		}
		Owl::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	OWL_PROFILE_FUNCTION();
	
	ImGui::Begin("Settings");
	{
		ImGui::ColorEdit4("Square Color", value_ptr(m_SquareColor));
		ImGui::SliderFloat("Square Rotation", &m_SquareRotation, 0, 360);
	}
	ImGui::End();
}

void Sandbox2D::OnEvent(Owl::Event& pEvent)
{
	m_CameraController.OnEvent(pEvent);
}
