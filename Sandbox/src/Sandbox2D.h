#pragma once
#include <Owl.h>

class EditorLayer : public Owl::Layer
{
public:
	EditorLayer();
	~EditorLayer() override = default;
	
	void OnAttach() override;
	void OnDetach() override;
	
	void OnUpdate(Owl::DeltaTime pDeltaTime) override;
	void OnImGuiRender() override;
	void OnEvent(Owl::Event& pEvent) override;

private:
	Owl::OrthographicCameraController m_CameraController;
	
	Owl::Ref<Owl::Texture2D> m_CheckerboardTexture;
	Owl::Ref<Owl::Texture2D> m_ChernoTexture;
	
	glm::vec4 m_SquareColor = {1.0f, 1.0f, 1.0f, 1.0f};
	float m_SquareRotation = 0.0f;
};
