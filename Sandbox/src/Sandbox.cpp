﻿#include <Owl.h>

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "Owl/Core/Input.h"
#include "Owl/Events/KeyEvent.h"

class ExampleLayer : public Owl::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f),
		m_SquarePosition(0.0f)
	{
		m_TriangleVertexArray.reset(Owl::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};

		Owl::Ref<Owl::VertexBuffer> triangleVertexBuffer;
		triangleVertexBuffer.reset(Owl::VertexBuffer::Create(vertices, sizeof(vertices)));
		triangleVertexBuffer->SetLayout({
			{ Owl::ShaderDataType::Float3, "in_Position" },
			{ Owl::ShaderDataType::Float4, "in_Color" }
		});
		m_TriangleVertexArray->AddVertexBuffer(triangleVertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		Owl::Ref<Owl::IndexBuffer> triangleIndexBuffer;
		triangleIndexBuffer.reset(Owl::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_TriangleVertexArray->SetIndexBuffer(triangleIndexBuffer);
		
		m_SquareVertexArray.reset(Owl::VertexArray::Create());
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f,
		};
		Owl::Ref<Owl::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Owl::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVertexBuffer->SetLayout({
			{ Owl::ShaderDataType::Float3, "in_Position" }
		});
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);
		
		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Owl::Ref<Owl::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Owl::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		std::string vertexSource = R"(
#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec4 in_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Color;

void main() 
{
	v_Color = in_Color;
	gl_Position = u_ViewProjection * u_Transform * vec4(in_Position, 1.0);
}
		)";
		
		std::string fragmentSource = R"(
#version 330 core

layout(location = 0) out vec4 out_Color;

in vec4 v_Color;

void main() 
{
	out_Color = v_Color;
}
		)";

		m_Shader = Owl::CreateScope<Owl::Shader>(vertexSource, fragmentSource);

		std::string vertexSourceBlue = R"(
#version 330 core

layout(location = 0) in vec3 in_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() 
{
	gl_Position = u_ViewProjection * u_Transform * vec4(in_Position, 1.0);
}
		)";
		
		std::string fragmentSourceBlue = R"(
#version 330 core

layout(location = 0) out vec4 out_Color;

void main() 
{
	out_Color = vec4(0.2, 0.2, 0.8, 1.0);
}
		)";

		m_BlueShader = Owl::CreateScope<Owl::Shader>(vertexSourceBlue, fragmentSourceBlue);
	}

	void OnUpdate(Owl::Timestep pTimestep) override
	{
		if (Owl::Input::IsKeyPressed(Owl::Key::Left))
			m_CameraPosition.x -= m_CameraMoveSpeed * pTimestep;
		if (Owl::Input::IsKeyPressed(Owl::Key::Right))
			m_CameraPosition.x += m_CameraMoveSpeed * pTimestep;
		if (Owl::Input::IsKeyPressed(Owl::Key::Up))
			m_CameraPosition.y += m_CameraMoveSpeed * pTimestep;
		if (Owl::Input::IsKeyPressed(Owl::Key::Down))
			m_CameraPosition.y -= m_CameraMoveSpeed * pTimestep;
		
		if (Owl::Input::IsKeyPressed(Owl::Key::A))
			m_CameraRotation += m_CameraRotationSpeed * pTimestep;
		if (Owl::Input::IsKeyPressed(Owl::Key::D))
			m_CameraRotation -= m_CameraRotationSpeed * pTimestep;
		
		if (Owl::Input::IsKeyPressed(Owl::Key::J))
			m_SquarePosition.x -= m_SquareMoveSpeed * pTimestep;
		if (Owl::Input::IsKeyPressed(Owl::Key::L))
			m_SquarePosition.x += m_SquareMoveSpeed * pTimestep;
		if (Owl::Input::IsKeyPressed(Owl::Key::I))
			m_SquarePosition.y += m_SquareMoveSpeed * pTimestep;
		if (Owl::Input::IsKeyPressed(Owl::Key::K))
			m_SquarePosition.y -= m_SquareMoveSpeed * pTimestep;
		
		Owl::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Owl::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Owl::Renderer::BeginScene(m_Camera);
		{
			static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f));

			for (int y = 0; y < 20; ++y)
			{
				for (int x = 0; x < 20; ++x)
				{
					glm::vec3 pos(x * 0.12f, y * 0.12f, 0.0f);
					glm::mat4 squareTransform = translate(glm::mat4(1.0f), pos) * scale;
					Owl::Renderer::Submit(m_SquareVertexArray, m_BlueShader, squareTransform);
				}
			}
			Owl::Renderer::Submit(m_TriangleVertexArray, m_Shader);
		}
		Owl::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{

	}

	void OnEvent(Owl::Event& pEvent) override
	{
	}
private:
	Owl::Ref<Owl::VertexArray> m_TriangleVertexArray;
	Owl::Ref<Owl::Shader> m_Shader;
		
	Owl::Ref<Owl::VertexArray> m_SquareVertexArray;
	Owl::Ref<Owl::Shader> m_BlueShader;

	Owl::OrthographicCamera m_Camera;
	
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;
	
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 2.5f;
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