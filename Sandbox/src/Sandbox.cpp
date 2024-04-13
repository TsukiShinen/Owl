﻿#include <Owl.h>

#include "imgui/imgui.h"

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

	void OnImGuiRender() override
	{
		ImGui::Begin("Example");
		ImGui::Text("Test");
		ImGui::End();
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
	}

	~Sandbox()
	{
		
	}
};

Owl::Application* Owl::CreateApplication()
{
	return new Sandbox();
}