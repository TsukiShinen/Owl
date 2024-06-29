#include <Owl.h>
#include <Owl/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace OwlEditor
{
    class OwlEditor : public Owl::Application
    {
    public:
        OwlEditor()
            : Owl::Application("Owl Editor")
        {
            PushLayer(new EditorLayer());
        }

        ~OwlEditor() override = default;
    };
    
}

Owl::Application* Owl::CreateApplication()
{
    return new OwlEditor::OwlEditor();
}
