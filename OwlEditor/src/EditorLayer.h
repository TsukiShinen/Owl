#pragma once
#include <Owl.h>

#include "Owl/Events/KeyEvent.h"
#include "Owl/Scene/Entity.h"
#include "Owl/Scene/SceneSerializer.h"
#include "Panels/SceneHierarchyPanel.h"

using namespace Owl;

namespace OwlEditor
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
        bool OnKeyPressed(const KeyPressedEvent& pEvent);

        void NewScene();
        void OpenScene();
        void SaveScene();
        void SaveSceneAs();

    private:
        OrthographicCameraController m_CameraController;
        
        Ref<Scene> m_ActiveScene;
        std::filesystem::path m_ActiveScenePath;
        Entity m_SquareEntity;
        Entity m_MainCameraEntity;
        Entity m_SecondCameraEntity;

        Entity m_HoveredEntity;
        
        bool m_PrimaryCamera;

        EditorCamera m_EditorCamera;
	
        Ref<Texture2D> m_CheckerboardTexture;
        
        bool m_ViewportFocused = false,  m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2];
        
        Ref<Framebuffer> m_Framebuffer;
	
        glm::vec4 m_SquareColor = {1.0f, 1.0f, 1.0f, 1.0f};
        float m_SquareRotation = 0.0f;

        int m_GizmoType = -1;

        SceneHierarchyPanel m_HierarchyPanel;
    };
    
}
