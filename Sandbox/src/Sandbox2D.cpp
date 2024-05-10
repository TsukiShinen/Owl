#include "Sandbox2D.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGlShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 780.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_SquareVertexArray = Owl::VertexArray::Create();
	float squareVertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
	};
	const Owl::Ref<Owl::VertexBuffer> squareVertexBuffer = Owl::VertexBuffer::Create(
		squareVertices, sizeof(squareVertices));
	squareVertexBuffer->SetLayout({
		{Owl::ShaderDataType::Float3, "in_Position"}
	});
	m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

	unsigned int squareIndices[6] = {0, 1, 2, 2, 3, 0};
	const Owl::Ref<Owl::IndexBuffer> squareIndexBuffer = Owl::IndexBuffer::Create(
		squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

	m_ShaderLibrary.Load("Assets/Shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
	Layer::OnDetach();
}

void Sandbox2D::OnUpdate(const Owl::DeltaTime pDeltaTime)
{
	m_CameraController.OnUpdate(pDeltaTime);

	Owl::RenderCommand::SetClearColor({.1f, .1f, .1f, 1});
	Owl::RenderCommand::Clear();

	Owl::Renderer::BeginScene(m_CameraController.GetCamera());
	{
		std::dynamic_pointer_cast<Owl::OpenGlShader>(m_ShaderLibrary.Get("FlatColor"))->Bind();
		std::dynamic_pointer_cast<Owl::OpenGlShader>(m_ShaderLibrary.Get("FlatColor"))->UploadUniformFloat4(
			"u_Color", m_SquareColor);
		
		Owl::Renderer::Submit(m_SquareVertexArray, m_ShaderLibrary.Get("FlatColor"), scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	}
	Owl::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	{
		ImGui::ColorEdit4("Square Color", value_ptr(m_SquareColor));
	}
	ImGui::End();
}

void Sandbox2D::OnEvent(Owl::Event& pEvent)
{
	m_CameraController.OnEvent(pEvent);
}
