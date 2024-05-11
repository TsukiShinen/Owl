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
	
	Owl::Ref<Owl::Texture2D> m_CheckerboardTexture;
	
	glm::vec4 m_SquareColor = {1.0f, 1.0f, 1.0f, 1.0f};
};
