#pragma once
#include "Owl/Core/Base.h"
#include "Owl/Scene/Entity.h"
#include "Owl/Scene/Scene.h"

using namespace Owl;

namespace OwlEditor
{
    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& pContext);

        void SetContext(const Ref<Scene>& pContext);
        
        void OnImGuiRender();
        
        [[nodiscard]] Entity GetSelectedEntity() const { return m_SelectionContext; }
    private:
        void DrawEntityNode(Entity pEntity);
        void DrawComponents(Entity pEntity);
    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
    
}
