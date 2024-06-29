#include "SceneHierarchyPanel.h"

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
            Entity entity{ pEntityId, m_Context.get() };
            DrawEntityNode(entity);
        });
        
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity pEntity)
    {
        auto& tag = pEntity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = (m_SelectionContext == pEntity) ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)pEntity, flags, tag.c_str());
        if (ImGui::IsItemClicked())
            m_SelectionContext = pEntity;

        if (opened)
        {
            ImGui::TreePop();
        }
    }
}
