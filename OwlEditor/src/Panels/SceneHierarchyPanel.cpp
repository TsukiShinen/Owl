#include "SceneHierarchyPanel.h"

#include <ranges>

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "ImGui/imgui_internal.h"
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

    static void DrawVector3Control(const std::string& pLabel, glm::vec3& pValues, float pResetValue = 0.0f, float pColumnWidth = 100.0f)
    {
        ImGui::PushID(pLabel.c_str());
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, pColumnWidth);
        ImGui::Text(pLabel.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.25f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
        if (ImGui::Button("X", buttonSize))
            pValues.x = pResetValue;
        ImGui::SameLine();
        ImGui::DragFloat("##X", &pValues.x, 0.1f, 0, 0, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleColor(3);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        if (ImGui::Button("Y", buttonSize))
            pValues.y = pResetValue;
        ImGui::SameLine();
        ImGui::DragFloat("##Y", &pValues.y, 0.1f, 0, 0, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleColor(3);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
        if (ImGui::Button("Z", buttonSize))
            pValues.z = pResetValue;
        ImGui::SameLine();
        ImGui::DragFloat("##Z", &pValues.z, 0.1f, 0, 0, "%.2f");
        ImGui::PopItemWidth();
        ImGui::PopStyleColor(3);
        
        ImGui::PopStyleVar();
        
        ImGui::Columns(1);

        ImGui::PopID();
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
                auto& transformComponent = pEntity.GetComponent<TransformComponent>();
                DrawVector3Control("Translation", transformComponent.Translation);
                glm::vec3 rotation = degrees(transformComponent.Rotation);
                DrawVector3Control("Rotation", rotation);
                transformComponent.Rotation = radians(rotation);
                DrawVector3Control("Scale", transformComponent.Scale, 1.0f);

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
