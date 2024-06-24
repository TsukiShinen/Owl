#include <Owl.h>
#include <Owl/Core/EntryPoint.h>


#include "Sandbox2D.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGlShader.h"

class Sandbox : public Owl::Application
{
public:
	Sandbox()
	{
		// PushLayer(new ExampleLayer());
		PushLayer(new EditorLayer());
	}

	~Sandbox() override = default;
};

Owl::Application* Owl::CreateApplication()
{
	return new Sandbox();
}
