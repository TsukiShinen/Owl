#pragma once
#include <Owl.h>

namespace Owl
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        ~EditorLayer() override = default;
	
        void OnAttach() override;
        void OnDetach() override;
	
        void OnUpdate(DeltaTime pDeltaTime) override;
        void OnImGuiRender() override;
        void OnEvent(Event& pEvent) override;

    private:
        OrthographicCameraController m_CameraController;
        
        Ref<Scene> m_ActiveScene;
        entt::entity m_SquareEntity;
	
        Ref<Texture2D> m_CheckerboardTexture;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        Ref<Framebuffer> m_Framebuffer;

        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;
	
        glm::vec4 m_SquareColor = {1.0f, 1.0f, 1.0f, 1.0f};
        float m_SquareRotation = 0.0f;
    };
    
}
