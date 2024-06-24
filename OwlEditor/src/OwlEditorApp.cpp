#include <Owl.h>
#include <Owl/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Owl
{
    class OwlEditor : public Application
    {
    public:
        OwlEditor()
            : Application("Owl Editor")
        {
            PushLayer(new EditorLayer());
        }

        ~OwlEditor() override = default;
    };

    Application* CreateApplication()
    {
        return new OwlEditor();
    }
    
}
