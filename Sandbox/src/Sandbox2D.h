#pragma once
#include <Owl.h>

class Sandbox2D : public Owl::Layer
{
public:
	Sandbox2D();
	~Sandbox2D() override = default;
	
	void OnAttach() override;
	void OnDetach() override;
	
	void OnUpdate(Owl::DeltaTime pDeltaTime) override;
	void OnImGuiRender() override;
	void OnEvent(Owl::Event& pEvent) override;

private:
	Owl::OrthographicCameraController m_CameraController;

	// Temp
	Owl::ShaderLibrary m_ShaderLibrary;
	Owl::Ref<Owl::VertexArray> m_SquareVertexArray;
	
	glm::vec4 m_SquareColor = {1.0f, 1.0f, 1.0f, 1.0f};
};
