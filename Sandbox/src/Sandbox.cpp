#include <Owl.h>
#include <Owl/Core/EntryPoint.h>


#include "Sandbox2D.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGlShader.h"

class ExampleLayer : public Owl::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 780.0f),
		  m_SquarePosition(0.0f)
	{
		m_TriangleVertexArray = Owl::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};

		Owl::Ref<Owl::VertexBuffer> triangleVertexBuffer = Owl::VertexBuffer::Create(vertices, sizeof(vertices));
		triangleVertexBuffer->SetLayout({
			{Owl::ShaderDataType::Float3, "in_Position"},
			{Owl::ShaderDataType::Float4, "in_Color"}
		});
		m_TriangleVertexArray->AddVertexBuffer(triangleVertexBuffer);

		unsigned int indices[3] = {0, 1, 2};
		Owl::Ref<Owl::IndexBuffer> triangleIndexBuffer = Owl::IndexBuffer::Create(
			indices, sizeof(indices) / sizeof(uint32_t));
		m_TriangleVertexArray->SetIndexBuffer(triangleIndexBuffer);

		m_SquareVertexArray = Owl::VertexArray::Create();
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		};
		Owl::Ref<Owl::VertexBuffer> squareVertexBuffer = Owl::VertexBuffer::Create(
			squareVertices, sizeof(squareVertices));
		squareVertexBuffer->SetLayout({
			{Owl::ShaderDataType::Float3, "in_Position"},
			{Owl::ShaderDataType::Float2, "in_TexCoord"}
		});
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		unsigned int squareIndices[6] = {0, 1, 2, 2, 3, 0};
		Owl::Ref<Owl::IndexBuffer> squareIndexBuffer = Owl::IndexBuffer::Create(
			squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		m_ShaderLibrary.Load("Assets/Shaders/VertexPositionColor.glsl");
		m_ShaderLibrary.Load("Assets/Shaders/FlatColor.glsl");
		auto textureShader = m_ShaderLibrary.Load("Assets/Shaders/Texture.glsl");

		m_Texture = Owl::Texture2D::Create("Assets/Textures/Checkerboard.png");
		m_TheChernoLogoTexture = Owl::Texture2D::Create("Assets/Textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Owl::OpenGlShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Owl::OpenGlShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(const Owl::DeltaTime pDeltaTime) override
	{
		m_CameraController.OnUpdate(pDeltaTime);

		if (Owl::Input::IsKeyPressed(Owl::Key::J))
			m_SquarePosition.x -= m_SquareMoveSpeed * pDeltaTime;
		if (Owl::Input::IsKeyPressed(Owl::Key::L))
			m_SquarePosition.x += m_SquareMoveSpeed * pDeltaTime;
		if (Owl::Input::IsKeyPressed(Owl::Key::I))
			m_SquarePosition.y += m_SquareMoveSpeed * pDeltaTime;
		if (Owl::Input::IsKeyPressed(Owl::Key::K))
			m_SquarePosition.y -= m_SquareMoveSpeed * pDeltaTime;

		Owl::RenderCommand::SetClearColor({.1f, .1f, .1f, 1});
		Owl::RenderCommand::Clear();

		Owl::Renderer::BeginScene(m_CameraController.GetCamera());
		{
			static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.08f));

			std::dynamic_pointer_cast<Owl::OpenGlShader>(m_ShaderLibrary.Get("FlatColor"))->Bind();
			std::dynamic_pointer_cast<Owl::OpenGlShader>(m_ShaderLibrary.Get("FlatColor"))->UploadUniformFloat3(
				"u_Color", m_SquareColor);

			for (int y = 0; y < 20; ++y)
				for (int x = 0; x < 20; ++x)
				{
					glm::vec3 pos(x * 0.1f, y * 0.1f, 0.0f);
					glm::mat4 squareTransform = translate(glm::mat4(1.0f), pos) * scale;
					Owl::Renderer::Submit(m_SquareVertexArray, m_ShaderLibrary.Get("FlatColor"), squareTransform);
				}

			m_Texture->Bind();
			Owl::Renderer::Submit(m_SquareVertexArray, m_ShaderLibrary.Get("Texture"),
			                      glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

			m_TheChernoLogoTexture->Bind();
			Owl::Renderer::Submit(m_SquareVertexArray, m_ShaderLibrary.Get("Texture"),
			                      glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

			// Triangle
			// Owl::Renderer::Submit(m_TriangleVertexArray, m_Shader);
		}
		Owl::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		{
			ImGui::ColorEdit3("Square Color", value_ptr(m_SquareColor));
		}
		ImGui::End();
	}

	void OnEvent(Owl::Event& pEvent) override
	{
		m_CameraController.OnEvent(pEvent);
	}

private:
	Owl::ShaderLibrary m_ShaderLibrary;

	Owl::Ref<Owl::VertexArray> m_TriangleVertexArray;
	Owl::Ref<Owl::VertexArray> m_SquareVertexArray;

	Owl::Ref<Owl::Texture2D> m_Texture, m_TheChernoLogoTexture;

	Owl::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 2.5f;

	glm::vec3 m_SquareColor = {1.0f, 1.0f, 1.0f};
};

class Sandbox : public Owl::Application
{
public:
	Sandbox()
	{
		// PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox() override = default;
};

Owl::Application* Owl::CreateApplication()
{
	return new Sandbox();
}
