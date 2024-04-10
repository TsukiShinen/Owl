#include <Owl.h>

class ExampleLayer : public Owl::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		
	}

	void OnUpdate() override
	{
		
	}

	void OnEvent(Owl::Event& pEvent) override
	{
		
	}
};

class Sandbox : public Owl::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushLayer(new Owl::ImGuiLayer());
	}

	~Sandbox()
	{
		
	}
};

Owl::Application* Owl::CreateApplication()
{
	return new Sandbox();
}