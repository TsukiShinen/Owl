#include "opch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGlShader.h"

namespace Owl
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_Storage;

	void Renderer2D::Init()
	{
		s_Storage = new Renderer2DStorage();
		s_Storage->QuadVertexArray = VertexArray::Create();

		float quadVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};
		Ref<VertexBuffer> quadVertexBuffer = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		quadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "in_Position"}
		});
		s_Storage->QuadVertexArray->AddVertexBuffer(quadVertexBuffer);

		uint32_t quadIndices[6] = {0, 1, 2, 2, 3, 0};
		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t));
		s_Storage->QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

		s_Storage->FlatColorShader = Shader::Create("Assets/Shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Storage;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& pCamera)
	{
		std::dynamic_pointer_cast<OpenGlShader>(s_Storage->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGlShader>(s_Storage->FlatColorShader)->UploadUniformMat4(
			"u_ViewProjection", pCamera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGlShader>(s_Storage->FlatColorShader)->UploadUniformMat4(
			"u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& pPosition, const glm::vec2& pSize, const glm::vec4& pColor)
	{
		DrawQuad({pPosition.x, pPosition.y, 0.0f}, pSize, pColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pPosition, const glm::vec2& pSize, const glm::vec4& pColor)
	{
		std::dynamic_pointer_cast<OpenGlShader>(s_Storage->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGlShader>(s_Storage->FlatColorShader)->UploadUniformFloat4("u_Color", pColor);

		s_Storage->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Storage->QuadVertexArray);
	}
}
