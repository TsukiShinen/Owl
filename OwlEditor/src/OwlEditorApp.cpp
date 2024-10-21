#include <Owl.h>
#include <Owl/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace OwlEditor
{
    class OwlEditor : public Application
    {
    public:
        OwlEditor(ApplicationCommandLineArgs pArgs)
            : Application("Owl Editor", pArgs)
        {
            PushLayer(new EditorLayer());
        }

        ~OwlEditor() override = default;
    };
    
}

Application* Owl::CreateApplication(ApplicationCommandLineArgs pArgs)
{
    return new OwlEditor::OwlEditor(pArgs);
}
