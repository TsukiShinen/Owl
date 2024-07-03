#include "SceneHierarchyPanel.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "Owl/Scene/Components.h"

namespace OwlEditor
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& pContext)
    {
        SetContext(pContext);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& pContext)
    {
        m_Context = pContext;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");

        m_Context->m_Registry.each([&](auto pEntityId)
        {
            Entity entity{pEntityId, m_Context.get()};
            DrawEntityNode(entity);
        });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};

        ImGui::End();

        ImGui::Begin("Properties");

        if (m_SelectionContext)
            DrawComponents(m_SelectionContext);

        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity pEntity)
    {
        auto& tag = pEntity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = (m_SelectionContext == pEntity)
                                       ? ImGuiTreeNodeFlags_Selected
                                       : 0 | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)pEntity, flags, tag.c_str());
        if (ImGui::IsItemClicked())
            m_SelectionContext = pEntity;

        if (opened)
            ImGui::TreePop();
    }

    void SceneHierarchyPanel::DrawComponents(Entity pEntity)
    {
        if (pEntity.HasComponent<TagComponent>())
        {
            auto& tag = pEntity.GetComponent<TagComponent>().Tag;

            char buffer[256] = {};
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        if (pEntity.HasComponent<TransformComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
                                  "Transform"))
            {
                auto& transform = pEntity.GetComponent<TransformComponent>().Transform;
                ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

                ImGui::TreePop();
            }
        }

        if (pEntity.HasComponent<CameraComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
            {
                auto& cameraComponent = pEntity.GetComponent<CameraComponent>();
                auto& camera = cameraComponent.Camera;
                
				ImGui::Checkbox("Primary", &cameraComponent.Primary);

                const char* projectionTypeStrings[] = {"Perspective", "Orthographic"};
                const char* currentProjectionTypeString = projectionTypeStrings[static_cast<int>(camera.GetProjectionType())];
                if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
                {
                    for (int i = 0; i < 2; i++)
                    {
                        const bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                        if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                        {
                            currentProjectionTypeString = projectionTypeStrings[i];
                            camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(i));
                        }

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }
                    
                    ImGui::EndCombo();
                }

                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
                {
                    float perspectiveFov = glm::degrees(camera.GetPerspectiveVerticalFov());
                    if (ImGui::DragFloat("Vertical FOV", &perspectiveFov))
                        camera.SetPerspectiveVerticalFov(glm::radians(perspectiveFov));
                    
                    float perspectiveNearClip = camera.GetPerspectiveNearClip();
                    if (ImGui::DragFloat("Near", &perspectiveNearClip))
                        camera.SetPerspectiveNearClip(perspectiveNearClip);
                    
                    float perspectiveFarClip = camera.GetPerspectiveFarClip();
                    if (ImGui::DragFloat("Far", &perspectiveFarClip))
                        camera.SetPerspectiveFarClip(perspectiveFarClip);
                }

                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
                {
                    float orthoSize = camera.GetOrthographicSize();
                    if (ImGui::DragFloat("Size", &orthoSize))
                        camera.SetOrthographicSize(orthoSize);
                    
                    float orthoNearClip = camera.GetOrthographicNearClip();
                    if (ImGui::DragFloat("Near", &orthoNearClip))
                        camera.SetOrthographicNearClip(orthoNearClip);
                    
                    float orthoFarClip = camera.GetOrthographicFarClip();
                    if (ImGui::DragFloat("Far", &orthoFarClip))
                        camera.SetOrthographicFarClip(orthoFarClip);

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
                }

                ImGui::TreePop();
            }
        }

        if (pEntity.HasComponent<SpriteRendererComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
                                  "Sprite Renderer"))
            {
                auto& spriteRendererComponent = pEntity.GetComponent<SpriteRendererComponent>();
                ImGui::ColorEdit4("Color", glm::value_ptr(spriteRendererComponent.Color));

                ImGui::TreePop();
            }
        }
    }
}
