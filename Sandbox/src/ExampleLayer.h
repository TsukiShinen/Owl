#pragma once
#include <Owl.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

class ExampleLayer : public Owl::Layer
{
public:
	ExampleLayer();

	void OnUpdate(Owl::DeltaTime pDeltaTime) override;
	void OnImGuiRender() override;
	void OnEvent(Owl::Event& pEvent) override;

private:
	Owl::ShaderLibrary m_ShaderLibrary;

	Owl::Ref<Owl::VertexArray> m_TriangleVertexArray;
	Owl::Ref<Owl::VertexArray> m_SquareVertexArray;

	Owl::Ref<Owl::Texture2D> m_Texture, m_TheChernoLogoTexture;

	Owl::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 2.5f;

	glm::vec3 m_SquareColor = {1.0f, 1.0f, 1.0f};
};
