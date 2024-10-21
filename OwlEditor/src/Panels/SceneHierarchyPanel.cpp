#include "SceneHierarchyPanel.h"

#include <ranges>

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "Owl/Scene/Components.h"

#ifdef _MSVC_LANG
  #define _CRT_SECURE_NO_WARNINGS
#endif

namespace Owl
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& pContext)
    {
        SetContext(pContext);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& pContext)
    {
        m_Context = pContext;
        m_SelectionContext = {};
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

        if (ImGui::BeginPopupContextWindow(nullptr, 1 | ImGuiPopupFlags_NoOpenOverItems))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
                m_Context->CreateEntity("New Entity");

            ImGui::EndPopup();
        }

        ImGui::End();

        ImGui::Begin("Properties");

        if (m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);
        }

        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity pEntity)
    {
        auto& tag = pEntity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = (m_SelectionContext == pEntity)
                                       ? ImGuiTreeNodeFlags_Selected
                                       : 0 | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanFullWidth;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)pEntity, flags, tag.c_str());
        if (ImGui::IsItemClicked())
            m_SelectionContext = pEntity;

        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
                entityDeleted = true;

            ImGui::EndPopup();
        }

        if (opened)
            ImGui::TreePop();

        if (entityDeleted)
        {
            m_Context->Destroy(pEntity);
            if (m_SelectionContext == pEntity)
                m_SelectionContext = {};
        }
    }

    static void DrawVector3Control(const std::string& pLabel, glm::vec3& pValues, float pResetValue = 0.0f,
                                   float pColumnWidth = 100.0f)
    {
        ImGui::PushID(pLabel.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, pColumnWidth);
        ImGui::Text(pLabel.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

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

    template<typename T, typename UiFunction>
    static void DrawComponent(const std::string& pName, Entity pEntity, UiFunction pUiFunction)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        if (pEntity.HasComponent<T>())
        {
            auto& component = pEntity.GetComponent<T>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
            
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, pName.c_str());
            ImGui::PopStyleVar();
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
            {
                ImGui::OpenPopup("ComponentSettings");
            }
            
            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove Component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }
            
            if (open)
            {
                pUiFunction(component);
                ImGui::TreePop();
            }

            if (removeComponent)
                pEntity.RemoveComponent<T>();
        }
    }

    void SceneHierarchyPanel::DrawComponents(Entity pEntity)
    {
        if (pEntity.HasComponent<TagComponent>())
        {
            auto& tag = pEntity.GetComponent<TagComponent>().Tag;

            char buffer[256] = {};
            std::strncpy(buffer, tag.c_str(), sizeof(buffer));
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        
        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");

        if (ImGui::BeginPopup("AddComponent"))
        {
            if (ImGui::MenuItem("Camera"))
            {
                if (!m_SelectionContext.HasComponent<CameraComponent>())
                    m_SelectionContext.AddComponent<CameraComponent>();
                else
                    OWL_CORE_WARN("This entity already has the Camera Component!");
                ImGui::CloseCurrentPopup();
            }
                
            if (ImGui::MenuItem("Sprite Renderer"))
            {
                if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
                    m_SelectionContext.AddComponent<SpriteRendererComponent>();
                else
                    OWL_CORE_WARN("This entity already has the Sprite Rendere rComponent!");
                ImGui::CloseCurrentPopup();
            }
                
            ImGui::EndPopup();
        }
        ImGui::PopItemWidth();

        DrawComponent<TransformComponent>("Transform", pEntity, [](auto& pComponent)
        {
            DrawVector3Control("Translation", pComponent.Translation);
            glm::vec3 rotation = degrees(pComponent.Rotation);
            DrawVector3Control("Rotation", rotation);
            pComponent.Rotation = radians(rotation);
            DrawVector3Control("Scale", pComponent.Scale, 1.0f);
        });

        DrawComponent<CameraComponent>("Camera", pEntity, [](auto& pComponent)
        {
            auto& camera = pComponent.Camera;

            ImGui::Checkbox("Primary", &pComponent.Primary);

            const char* projectionTypeStrings[] = {"Perspective", "Orthographic"};
            const char* currentProjectionTypeString = projectionTypeStrings[static_cast<int>(camera.
                GetProjectionType())];
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

                ImGui::Checkbox("Fixed Aspect Ratio", &pComponent.FixedAspectRatio);
            }
        });
        
        DrawComponent<SpriteRendererComponent>("Sprite Renderer", pEntity, [](auto& pComponent)
        {
            ImGui::ColorEdit4("Color", glm::value_ptr(pComponent.Color));
        });
    }
}
